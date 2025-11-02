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

// MARK: cycle image
let imagePupe = ["../image/member/pupe1.png", "../image/member/pupe2.png"];
let imageOffice = ["../image/member/office1.png", "../image/member/office2.png"];
let imageArse = ["../image/member/arse1.png", "../image/member/arse2.png"];
let imageKia = ["../image/member/kia1.png", "../image/member/kia2.png"];
let index = 0;
const intervalTime = 3000;

function cycleImage()
{
    index = (index + 1) % 2;
    $("#pupe").attr("src", imagePupe[index]);
    $("#office").attr("src", imageOffice[index]);
    $("#arse").attr("src", imageArse[index]);
    $("#kia").attr("src", imageKia[index]);
}

$(document).ready(function()
{
    setInterval(cycleImage, intervalTime);
});
