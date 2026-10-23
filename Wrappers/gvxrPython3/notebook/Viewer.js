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
        console.log("widget focused");
        wrapper.focus();
    });

    wrapper.addEventListener("focus", () => {
        console.log("focus");
    });

    wrapper.addEventListener("blur", () => {
        console.log("blur");
    });

    let key_event_id = 0;
    let mouse_event_id = 0;

    wrapper.addEventListener("contextmenu", (e) => {
        console.log("contextmenu");

        e.preventDefault();
        e.stopPropagation();
        return false;
    }, true);

    img.addEventListener("contextmenu", (e) => {
        console.log("img contextmenu");

        e.preventDefault();
        e.stopPropagation();
        return false;
    }, true);

    wrapper.addEventListener("keydown", (e) => {
        try {
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

        model.set("mouse_x", e.offsetX);
        model.set("mouse_y", e.offsetY);

        model.set("mouse_action", "mousemove");

        model.set("mouse_event_id", ++mouse_event_id);

        model.save_changes();
    });

    wrapper.addEventListener("wheel", (e) => {

        console.log(e)
        console.log(e.deltaX)
        console.log(e.deltaY)
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

        img.src = URL.createObjectURL(blob);
    }

    model.on("change:png", update);

    update();

    // Optional: grab focus initially
    wrapper.focus();
}

export default { render };