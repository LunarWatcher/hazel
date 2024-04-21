function refreshLinks(links) {
    let targetContainer = document.getElementById("links");
    if (targetContainer == null) {
        console.log("Failed to get link container");
        return false;
    }

    for (let link of links) {
        // Unsafe?
        let elem = document.querySelector("[data-title=\"" + link.title + "\"]");
        if (elem == null) {
            // Link container
            elem = targetContainer.appendChild(document.createElement("div"));
            elem.setAttribute("data-title", link.title);
            elem.classList.add("small-paragraphs", "link-container");

            // Logo and content container
            let titleLink = elem.appendChild(document.createElement("a"));
            titleLink.href = encodeURI(link.url);
            titleLink.classList.add("flex-horizontal");

            
            let left = titleLink.appendChild(document.createElement("div"));
            left.classList.add("link-logo");

            let right = titleLink.appendChild(document.createElement("div"));
            right.classList.add("link-content");

            let top = right.appendChild(document.createElement("div"));
            top.classList.add("link-top");

            let title = top.appendChild(document.createElement("p"));
            title.classList.add("link-title");
            title.innerText = link.title;


            
        }

        let fields = elem.getElementsByClassName("link-fields");
        if (fields == null || fields.length != 1) {
            if (Object.keys(link.fields).length > 0) {
                // (Re)create the field container
                let bottom = elem.getElementsByClassName("link-content")[0].appendChild(document.createElement("div"));
                bottom.classList.add("link-fields", "flex-horizontal");
                fields = [bottom];
            } else {
                // The container doesn't exist, and there aren't any links; everything is fine, continue
                continue;
            }
        }

        let fieldContainer = fields[0];

        if (link.fields && Object.keys(link.fields).length > 0) {
            for (const [field, value] of Object.entries(link.fields)) {
                let fieldElem = fieldContainer.querySelector("[data-field-title=\"" + field + "\"]");
                if (fieldElem == null) {
                    fieldElem = fieldContainer.appendChild(document.createElement("div"));
                    fieldElem.classList.add("link-field");
                    let title = fieldElem.appendChild(document.createElement("p"));
                    title.setAttribute("data-field-title", field);
                    title.innerText = field;
                    title.classList.add("field-title");

                    let valueField = fieldElem.appendChild(document.createElement("p"));
                    valueField.classList.add("field-value");
                }
                fieldElem.getElementsByClassName("field-value")[0].innerText = value;
            }
        } else {
            // Wipe fields if the server suddenly returns none
            fieldContainer.remove();
        }

    }

    return true;
}

function updateDashboard() {
    fetch("/api/dashboard")
        .then((res) => {
            // TODO handle bad status code
            return res.json();
        })
        .then((json) => {
            let anyFound = false;
            if ("links" in json && json["links"] != null) {
                anyFound = refreshLinks(json["links"]) || anyFound;
            }

            if (!anyFound) {
                console.log("No elements found; killing interval");
                clearInterval(dashInterval);
            }
        });
}

updateDashboard();
var dashInterval = setInterval(updateDashboard, 60000);
