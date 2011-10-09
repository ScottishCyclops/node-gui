#include <gtk/gtk.h>
#include <stdio.h>

#include "node_gui_object.h"
#include "node_gui_widget.h"
#include "impl_mainloop_gtk.h"

namespace clip {
Persistent<FunctionTemplate> Widget::constructor_template;

void Widget::Init (Handle<v8::Object> target) {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New (New);
    constructor_template = Persistent<FunctionTemplate>::New(t);
    constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
    constructor_template->SetClassName(String::NewSymbol("Widget"));
    constructor_template->Inherit (Object::constructor_template);

    NODE_SET_PROTOTYPE_METHOD (constructor_template, "show", Show);
    NODE_SET_PROTOTYPE_METHOD (constructor_template, "destroy", Destroy);
    target->Set (String::NewSymbol ("Widget"), t->GetFunction ());
}

Handle<Value> Widget::New (const Arguments& args) {
    HandleScope scope;

    // Init from exsiting gtk::widget
    if (args.Length () == 1 && args[0]->IsExternal ()) {
        void *obj = External::Unwrap (args[0]);
        Widget *self = new Widget (obj);

        self->Wrap (args.This ());
        return args.This ();
    }

    return ThrowException(Exception::TypeError(String::New(
                    "Widget is not allow to be manually created")));
}

Handle<Value> Widget::Show (const Arguments& args) {
    HandleScope scope;

    Widget *self = ObjectWrap::Unwrap<Widget> (args.This());
    GtkWidget *obj = static_cast<GtkWidget*> (self->obj_);

    MainLoop::push_job_gui ([=] {
        gtk_widget_show_all (obj);
    });

    return Undefined ();
}

Handle<Value> Widget::Destroy (const Arguments& args) {
    HandleScope scope;

    Widget *self = ObjectWrap::Unwrap<Widget> (args.This());
    GtkWidget *obj = static_cast<GtkWidget*> (self->obj_);

    MainLoop::push_job_gui ([=] {
        gtk_widget_destroy (obj);
    });

    return Undefined ();
}
} /* clip */
