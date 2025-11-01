import { repaintChart } from "./firebase.js";

// MARK: apply theme
function applyTheme(theme)
{
    if (theme === "auto")
    {
        theme = window.matchMedia('(prefers-color-scheme: dark)').matches ? "dark" : "light";
    }
    $("html").attr("data-bs-theme", theme);
    repaintChart(theme);
}

// MARK: load saved theme
let savedTheme = localStorage.getItem("theme") || "auto";
if (savedTheme === "dark")
{
    $("#radioDefault1").prop("checked", true);
}
else if (savedTheme === "light")
{
    $("#radioDefault2").prop("checked", true);
}
else
{
    $("#radioDefault3").prop("checked", true);
}
applyTheme(savedTheme);

// MARK: switch theme
$("#radioDefault1").click(function()
{
    localStorage.setItem("theme", "dark");
    applyTheme("dark");
});

$("#radioDefault2").click(function()
{
    localStorage.setItem("theme", "light");
    applyTheme("light");
});

$("#radioDefault3").click(function()
{
    localStorage.setItem("theme", "auto");
    applyTheme("auto");
});
