function render({ model, el }) {

    console.log("gVXR widget loaded");

    const wrapper = document.createElement("div");
    wrapper.tabIndex = 0;
    wrapper.style.display = "inline-block";
    wrapper.style.outline = "1px solid red";

    // JupyterLab binds the arrow keys to moving from cell to cell, and acts on
    // them from the document before they reach a listener on this element. The
    // widget would see N for example, which is bound to nothing, but never an
    // arrow. These attributes tell Lumino, and PhosphorJS in the older versions,
    // to leave the keyboard alone over this element
    wrapper.setAttribute("data-lm-suppress-shortcuts", "true");
    wrapper.setAttribute("data-p-suppress-shortcuts", "true");

    const img = document.createElement("img");
    img.style.display = "block";

    wrapper.appendChild(img);

    el.appendChild(wrapper);

    wrapper.addEventListener("click", () => {
        wrapper.focus();
    });

    // Show whether the keyboard is going to reach the widget
    wrapper.addEventListener("focus", () => {
        wrapper.style.outline = "2px solid red";
    });

    wrapper.addEventListener("blur", () => {
        wrapper.style.outline = "1px solid grey";
    });

    let key_event_id = 0;
    let mouse_event_id = 0;

    // The keys currently held down, so that they can be released when the
    // widget loses the focus, see below
    const keys_held = new Set();

    // Whether the S or the D key is held down. gVXR reacts to a move with no
    // button pressed only in that case, to change the source to object or the
    // object to detector distance
    let sd_held = false;

    function sendKey(key, action, ctrl, shift, alt) {

        model.set("key", key);
        model.set("action", action);

        model.set("ctrl", ctrl);
        model.set("shift", shift);
        model.set("alt", alt);

        model.set("key_event_id", ++key_event_id);

        model.save_changes();
    }

    // gVXR remembers which keys are down, and only ever forgets one when it
    // is told the key went up. A key held while the widget loses the focus
    // would never be released, and would go on altering what the next key
    // press does, so release them all here
    wrapper.addEventListener("blur", () => {

        for (const key of keys_held)
            sendKey(key, "keyup", false, false, false);

        keys_held.clear();
        sd_held = false;
    });

    wrapper.addEventListener("contextmenu", (e) => {

        e.preventDefault();
        e.stopPropagation();
        return false;
    }, true);

    img.addEventListener("contextmenu", (e) => {

        e.preventDefault();
        e.stopPropagation();
        return false;
    }, true);

    wrapper.addEventListener("keydown", (e) => {
        try {
            if (e.key === "s" || e.key === "S" || e.key === "d" || e.key === "D")
                sd_held = true;

            keys_held.add(e.key);

            model.set("key", e.key);
            model.set("action", "keydown");

            model.set("ctrl", e.ctrlKey);
            model.set("shift", e.shiftKey);
            model.set("alt", e.altKey);

            model.set("key_event_id", ++key_event_id);

            model.save_changes();

            // Keep the key for the widget. Without stopPropagation it carries
            // on up to the notebook, which would act on it a second time
            e.preventDefault();
            e.stopPropagation();
        }
        catch (err) {
            console.error(err);
        }
    });

     wrapper.addEventListener("keyup", (e) => {
        try {
            if (e.key === "s" || e.key === "S" || e.key === "d" || e.key === "D")
                sd_held = false;

            keys_held.delete(e.key);

            model.set("key", e.key);
            model.set("action", "keyup");

            model.set("ctrl", e.ctrlKey);
            model.set("shift", e.shiftKey);
            model.set("alt", e.altKey);

            model.set("key_event_id", ++key_event_id);

            model.save_changes();

            // Keep the key for the widget. Without stopPropagation it carries
            // on up to the notebook, which would act on it a second time
            e.preventDefault();
            e.stopPropagation();
        }
        catch (err) {
            console.error(err);
        }
    });

    wrapper.addEventListener("mousedown", (e) => {

        model.set("mouse_x", e.offsetX);
        model.set("mouse_y", e.offsetY);

        model.set("mouse_button", e.button);
        model.set("mouse_action", "mousedown");

        model.set("mouse_event_id", ++mouse_event_id);

        model.save_changes();

        e.preventDefault();
        e.stopPropagation();
        wrapper.focus();
    });

    wrapper.addEventListener("mouseup", (e) => {

        model.set("mouse_x", e.offsetX);
        model.set("mouse_y", e.offsetY);

        model.set("mouse_button", e.button);
        model.set("mouse_action", "mouseup");

        model.set("mouse_event_id", ++mouse_event_id);

        model.save_changes();

        e.preventDefault();
    });

    wrapper.addEventListener("mousemove", (e) => {

        // Hovering over the widget changes nothing in the scene, so sending
        // the move would cost a render and a round trip only to produce the
        // very same image
        if (e.buttons === 0 && !sd_held)
            return;

        model.set("mouse_x", e.offsetX);
        model.set("mouse_y", e.offsetY);

        model.set("mouse_action", "mousemove");

        model.set("mouse_event_id", ++mouse_event_id);

        model.save_changes();
    });

    wrapper.addEventListener("wheel", (e) => {

        model.set("wheel_dx", Math.sign(e.deltaX));
        model.set("wheel_dy", Math.sign(e.deltaY));

        model.set("mouse_action", "wheel");

        model.set("mouse_event_id", ++mouse_event_id);

        model.save_changes();

        e.preventDefault();
    });

    function update() {

        const png = model.get("png");

        if (!png)
            return;

        const blob = new Blob(
            [png],
            { type: "image/png" }
        );

        // An object URL stays alive until it is revoked, so without this the
        // browser would hold on to every frame ever displayed. The previous
        // one is released only once the new one has been decoded, otherwise
        // the image would flicker
        const previous = img.src;

        img.onload = () => {
            if (previous.startsWith("blob:"))
                URL.revokeObjectURL(previous);
        };

        img.src = URL.createObjectURL(blob);
    }

    model.on("change:png", update);

    update();

    // Grab the focus initially. It has to wait for the next frame: calling it
    // here would be undone by the notebook, which puts the focus back on the
    // cell once its output has been rendered
    requestAnimationFrame(() => wrapper.focus());
}

export default { render };