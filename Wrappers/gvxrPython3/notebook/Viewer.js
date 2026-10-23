function render({ model, el }) {

    console.log("gVXR widget loaded");

    const wrapper = document.createElement("div");
    wrapper.tabIndex = 0;
    wrapper.style.display = "inline-block";
    wrapper.style.outline = "1px solid red";

    const img = document.createElement("img");
    img.style.display = "block";

    wrapper.appendChild(img);

    el.appendChild(wrapper);

    wrapper.addEventListener("click", () => {
        wrapper.focus();
    });

    // wrapper.addEventListener("focus", () => {
    //     console.log("focus");
    // });
    //
    // wrapper.addEventListener("blur", () => {
    //     console.log("blur");
    // });

    let key_event_id = 0;
    let mouse_event_id = 0;

    // Whether the S or the D key is held down. gVXR reacts to a move with no
    // button pressed only in that case, to change the source to object or the
    // object to detector distance
    let sd_held = false;

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

            model.set("key", e.key);
            model.set("action", "keydown");

            model.set("ctrl", e.ctrlKey);
            model.set("shift", e.shiftKey);
            model.set("alt", e.altKey);

            model.set("key_event_id", ++key_event_id);

            model.save_changes();

            e.preventDefault();
        }
        catch (err) {
            console.error(err);
        }
    });

     wrapper.addEventListener("keyup", (e) => {
        try {
            if (e.key === "s" || e.key === "S" || e.key === "d" || e.key === "D")
                sd_held = false;

            model.set("key", e.key);
            model.set("action", "keyup");

            model.set("ctrl", e.ctrlKey);
            model.set("shift", e.shiftKey);
            model.set("alt", e.altKey);

            model.set("key_event_id", ++key_event_id);

            model.save_changes();

            e.preventDefault();
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

    // Optional: grab focus initially
    wrapper.focus();
}

export default { render };