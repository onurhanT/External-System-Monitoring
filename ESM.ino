#include <ros.h>
#include <std_msgs/String.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);
ros::NodeHandle nh;
int switch_button = 2;
int screen_state = 0;
String messages[5] = {};

void switch_state(){
  if(digitalRead(switch_button) == HIGH){
    screen_state++;
    screen_state = screen_state % 4;
  }
  screen_manager();
}

void display_menu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A-System Usage");
  lcd.setCursor(0,1);
  lcd.print("B-Battery & Heat");
}

void display_usage(){
  lcd.clear();
  lcd.setCursor(0,0);
  String CPU = "CPU:" + messages[0];
  lcd.print(CPU);
  lcd.setCursor(8,0);
  String MEM = "MEM:" + messages[1];
  lcd.print(MEM);
  lcd.setCursor(0,1);
  String Disk = "Disk:" + messages[2];
  lcd.print(Disk);
}

void display_bh(){
  lcd.clear();
  lcd.setCursor(0,0);
  String Battery = "Battery :" + messages[3] + "%";
  lcd.print(Battery);
  lcd.setCursor(0,1);
  String Heat = "Heat Avg:" + messages[4] + "C";
  lcd.print(Heat);
}

void display_availibility(){
  if(nh.connected()){
    lcd.clear();
    lcd.print("Ready to Read...");
  }else{
    lcd.clear();
    lcd.print("Waiting...");
  }
  
}

void screen_manager(){
  if(screen_state == 0){
    //Availibility state
    display_availibility();
  }else if(screen_state == 1){
    //Menu
    display_menu();
  }else if(screen_state == 2){
    //System Usage
    display_usage();
  }else if(screen_state == 3){
    //Battery&Heat
    display_bh();
  }
}

void handle_msg(const std_msgs::String& msg)
{
  //Parse ROS Message according to order (Cpu,Mem,Disk,Battery,Heat) and use Delimeter "/"
  String output = msg.data;
  String CPU = output.substring(0, output.indexOf("/"));
  String Rest = output.substring(output.indexOf("/") + 1);  
  String MEM = Rest.substring(0, Rest.indexOf("/"));   
  Rest =  Rest.substring(Rest.indexOf("/") + 1);    
  String Disk = Rest.substring(0, Rest.indexOf("/")); 
  Rest =  Rest.substring(Rest.indexOf("/") + 1);
  String Battery =  Rest.substring(0, Rest.indexOf("/")); 
  Rest =  Rest.substring(Rest.indexOf("/") + 1);
  String Heat =  Rest.substring(0, Rest.indexOf("/")); 
  //Set input data into the messages array with given order
  messages[0] = CPU;
  messages[1] = MEM;
  messages[2] = Disk;
  messages[3] = Battery;
  messages[4] = Heat;
}

//Init ROS subscriber and sub node name sysytem_monitor
ros::Subscriber<std_msgs::String> sub("system_monitor", &handle_msg);

void setup()
{    
  //Init LCD 16x2 I2C
  lcd.init();
  lcd.backlight();
  lcd.print("Setting up...");
  lcd.blink();
  //Init 3 Button
  pinMode(switch_button, INPUT);
  //Init ROS Subscriber
  nh.initNode();
  nh.subscribe(sub);
  delay(100);
}

void loop()
{
  nh.spinOnce();
  switch_state();
  delay(300);
}
