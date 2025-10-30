const savedTheme = localStorage.getItem("theme");
$("html").attr("data-bs-theme", savedTheme);
$("#switch").text(savedTheme === "dark" ? "Light Theme" : "Dark Theme");
$("#switch").click(function()
{
    let oldTheme = $("html").attr("data-bs-theme");
    let newTheme = (oldTheme === "dark" ? "light" : "dark");
    $("html").attr("data-bs-theme", newTheme);
    $("#switch").text(newTheme === "dark" ? "Light Theme" : "Dark Theme");
    localStorage.setItem("theme", newTheme);
});
