extern crate serialport;

use serde_derive::{Deserialize, Serialize};
use serialport::prelude::*;
use std::{thread, time};

#[derive(Serialize, Deserialize)]
struct Msg {
    wheel_speed: u8,
    left_side_dir: String,
    right_side_dir: String,
}

fn main() {
    let msg = Msg{
            wheel_speed : 100.to_owned(),
            left_side_dir : "Forward".to_owned(),
            right_side_dir : "Forward".to_owned()
    };

    let j_str = serde_json::to_string(&msg).unwrap();
    // Send to Serial port
    println!("{}", j_str);

    let mut settings: SerialPortSettings = Default::default();
    settings.baud_rate = 9600;

    let default_port_name = "/dev/ttyACM0";
    match serialport::open_with_settings(&default_port_name, &settings){
        Ok(mut port) => {
            println!("Opened port {:?}, sleeping for 3 sec", port.name().unwrap());
            thread::sleep(time::Duration::from_secs(3));
            println!("Awake");
            match port.write(j_str.as_bytes()) {
                Ok(_) => println!("Successfully printed JSON"),
                Err(e) => eprintln!("Could not print JSON, {:?}", e),
            }
        },
        Err(e) => eprintln!("Could not open port at {}, {:?}", &default_port_name, e.description),
    }
}
