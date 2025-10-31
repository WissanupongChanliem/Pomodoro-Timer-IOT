// Import the functions you need from the SDKs you need
import { initializeApp } from "https://www.gstatic.com/firebasejs/12.4.0/firebase-app.js";
import { getDatabase, ref, get, child, onValue } from "https://www.gstatic.com/firebasejs/12.4.0/firebase-database.js";

// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries
// Your web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyC6vYMSD4HvgwI_fHzmT8eV4tFfqTWcNis",
    authDomain: "pomodoroiot.firebaseapp.com",
    databaseURL: "https://pomodoroiot-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "pomodoroiot",
    storageBucket: "pomodoroiot.firebasestorage.app",
    messagingSenderId: "11062092109",
    appId: "1:11062092109:web:f191926448a30e298a1704"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);


const sessionCountRef = ref(db, "/Session");
const sessionsRef = ref(db, "/");
let chart = null;
let chartData = [];

onValue(sessionCountRef, (snapshot) => {
  const data = snapshot.val();
  $("#session-count-text").text(data);
});

onValue(sessionsRef, (snapshot) => {
    let totalTime = 0;
    chartData = []
    snapshot.forEach((childSnapshot) => {
        const childKey = childSnapshot.key;
        const childData = childSnapshot.val();
        if (childKey!="Session")
        {
            if (childData.round_studied)
            {
                totalTime += childData.round_studied * 25;
                chartData.push([parseInt(childKey.slice(7)), childData.round_studied * 25]);
            }
            else{
                chartData.push([parseInt(childKey.slice(7)), 0]);
            }
        }
    });
    chartData.sort((a, b) => {
        return a[0] - b[0];
    });
    updateChart();
    if (totalTime > 60)
    {
        $("#study-time-text").text((totalTime/60).toFixed(2) + " hours");
    }
    else{
        $("#study-time-text").text(totalTime + " minutes");
    }
    
});

async function updateChart(){
    let canvas = document.getElementById('usage');

    if(chart){
        chart.clear();
        chart.destroy();
    }
    chart = new Chart(
        canvas,
        {
        type: 'bar',
        data: {
            labels: chartData.map(row => row[0]),
            datasets: [
            {
                label: 'Time (minutes) spend on each session',
                data: chartData.map(row => row[1]),
                backgroundColor: '#fc535c'
            }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
        }
    );
}



export function repaintChart(theme) {
    
    Chart.defaults.color = theme === "dark" ? "#fff" : "#666";
    Chart.defaults.borderColor = theme === "dark" ? "rgba(255, 255, 255, 0.3)" : "rgba(0, 0, 0, 0.1)";
    updateChart();
}