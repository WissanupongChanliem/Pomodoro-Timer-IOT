# Physical Computing Project 2025 - IT KMITL

![](/image/pomothink/pomothink_0.png)

# PomoThink 🍅
> Smart Pomodoro Timer System using IoT for Productivity Enhancement

**PomoThink** คือโครงงาน **Pomodoro Timer**  บนบอร์ด **Arduino UNO R4 WiFi** ที่ออกแบบมาเพื่อช่วยเพิ่มสมาธิในการเรียนรู้และทำงาน โดยใช้หลักการของเทคนิค **Pomodoro Technique** ซึ่งเป็นการแบ่งเวลาทำงานออกเป็นรอบ ๆ เพื่อเพิ่มประสิทธิภาพและลดความเหนื่อยล้า

## 🕒 About Pomodoro Timer
เทคนิค **Pomodoro** จะใช้ช่วงเวลาทำงาน (Working Session) สลับกับช่วงเวลาพัก (Break Session) โดยทั่วไปจะมีรูปแบบดังนี้:
- ทำงาน 25 นาที
- พักสั้น (Short Break) 5 นาที
- พักยาว (Long Break) 15 นาที หลังจากครบ 4 รอบ

PomoThink จะช่วยจัดการเวลาเหล่านี้ให้อัตโนมัติ พร้อมทั้งแสดงสถานะและตรวจจับพฤติกรรมการเรียนรู้ของผู้ใช้

## ⚙️ Components Used
- 🟦 **Arduino UNO R4 WiFi**
- 💡 **LED** (แสดงสถานะการทำงาน)
  - ไม่มีสี → **WORKING**
  - สีแดง → **STOPPED**
  - สีเขียว → **SHORT_BREAK**
  - สีฟ้า → **LONG_BREAK**
- 📟 **LiquidCrystal LCD** (แสดงเวลาและสถานะปัจจุบัน)
- 📏 **Ultrasonic Sensor** (ตรวจจับว่าผู้ใช้นั่งอยู่หน้าคอม/โต๊ะหรือไม่)
- 🔘 **Button 2 ปุ่ม**
  - `STOP` → หยุดการทำงานชั่วคราว
  - `RESTART` → เริ่มรอบใหม่

## ☁️ Firebase Connection
PomoThink เชื่อมต่อกับ **Firebase Realtime Database** ผ่าน WiFi ของบอร์ด Arduino UNO R4 WiFi
โดยมีการเก็บข้อมูลและสถิติต่าง ๆ ได้แก่:
- จำนวนรอบที่ทำงานเสร็จแล้ว
- เวลาทำงานรวมทั้งหมด
- จำนวนครั้งที่สูญเสียสมาธิ
นอกจากนี้ยังสามารถแสดงผลสถิติผ่าน website ที่เชื่อมกับ Firebase ได้

## 👀 Main Features
- ⏱️ **ระบบจับเวลา Pomodoro อัตโนมัติ**
- 📊 **เชื่อมต่อ Firebase เพื่อเก็บข้อมูลสถิติ**
- 🧍 **ตรวจจับการนั่งอยู่กับที่ด้วย Ultrasonic Sensor**
- 📺 **แสดงผลบนหน้าจอ LCD**
- 💡 **LED แสดงสถานะการทำงานแบบเรียลไทม์**
- 🔄 **ปุ่ม STOP / RESTART สำหรับควบคุมรอบการทำงาน**

## 🎨 Poster
![](/image/poster/pomothink_poster.png)

## 🚀 Languages, Framework & Tool
<img src="https://skillicons.dev/icons?i=arduino,html,css,js,bootstrap,jquery,firebase"/>

## 🧑‍💻 Contributors
* [67070094](https://github.com/CyanKagami) บุญสิตา จงกลม
* [67070164](https://github.com/GEOFFCHARGE) วัลชัย กล่ำม่วง
* [67070167](https://github.com/WissanupongChanliem) วิษณุพงศ์ จันทร์เลี่ยม
* [67070179](https://github.com/67070179Smith) สมิต สารบรรณ
