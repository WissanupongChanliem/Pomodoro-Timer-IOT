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
let image1 = ["../image/member/pupe1.png", "../image/member/pupe2.png"];
let image2 = ["../image/member/office1.png", "../image/member/office2.png"];
let image3 = ["../image/member/arse1.png", "../image/member/arse2.png"];
let image4 = ["../image/member/kia1.png", "../image/member/kia2.png"];
let index = 0;

function cycleImage()
{
    index = (index + 1) % 2;
    $("#image1").fadeOut(400, function()
    {
        $(this).attr("src", image1[index]).fadeIn(400);
    });
    $("#image2").fadeOut(400, function()
    {
        $(this).attr("src", image2[index]).fadeIn(400);
    });
    $("#image3").fadeOut(400, function()
    {
        $(this).attr("src", image3[index]).fadeIn(400);
    });
    $("#image4").fadeOut(400, function()
    {
        $(this).attr("src", image4[index]).fadeIn(400);
    });
}

$(document).ready(function()
{
    setInterval(cycleImage, 3000);
});
