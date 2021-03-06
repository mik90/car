extern crate evdev_rs as evdev;
extern crate nix;
extern crate serialport;

use std::{thread, time};
use std::net::UdpSocket;
use nix::sys::socket::SockAddr;
use nix::sys::socket::AddressFamily;

mod ds4_device;
use ds4_device::Ds4Device;
mod json_interface;
use json_interface::*;

fn get_interface_addr_str() -> Option<SockAddr> {
    let mut addrs = nix::ifaddrs::getifaddrs().unwrap();
    match addrs.find(|x| x.interface_name == "wlp3s0" 
                        && x.address.unwrap().family() == AddressFamily::Inet) {
        Some(interface) => interface.address,
        None => None,
    }
}

fn main()
{
    let mut args = std::env::args();
    if args.len() != 2 {
        println!("Usage: ds4_input /path/to/event/device");
        std::process::exit(1);
    }

    let dev_path = &args.nth(1).unwrap(); // Grab the device path
    println!("Device path: {:?}:", dev_path);

    let mut ds4_dev = Ds4Device::new(dev_path).unwrap();

    let interval = time::Duration::from_millis(20);
    println!("Updating values every {:?} milliseconds", interval);
    let mut iteration = 0;
    let mut time_since_last_update = time::Instant::now();

    let mut addr_str = get_interface_addr_str()
                        .unwrap()
                        .to_str();
    addr_str.pop(); // The normal port is :0 and we want :50001
    addr_str.push_str("50001");

    println!("Trying to bind to {}", addr_str);
    let socket = UdpSocket::bind(addr_str).expect("Couldn't bind UDP socket");
    let dest = "192.168.1.167:50001";
    println!("Bind() succeeded.");

    println!("Starting main loop in 2 seconds..");
    thread::sleep(time::Duration::from_secs(2));

    let mut last_msg = Msg::new();
    // Main event loop
    loop {
        println!("Iteration:{}    ---------------------------------------------------------",
                 iteration);
        iteration += 1;
        ds4_dev.update_values();

        let now = time::Instant::now();

        // Only write to the serial port periodically
        if now - time_since_last_update > interval {
            time_since_last_update = now;

            let msg = ds4_device_to_msg(&ds4_dev);
            if !msg_is_duplicate(&msg, &last_msg) {
                // Only send a message if there's a new value that's
                // significantly different from the last one that was sent
                let json = msg_to_json_string(&msg);
                match socket.send_to(json.as_bytes(), dest) {
                    Ok(v) => println!("Wrote {} bytes to {}", v, dest),
                    Err(e) => eprintln!("Error:{}, couldn't write to {}", e, dest),
                }
                last_msg = msg;
            }
        }
    }
}