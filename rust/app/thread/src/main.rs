use std::thread;
use std::time::Duration;
use std::sync::mpsc;
use std::sync::mpsc::{Sender, Receiver};

fn main() {
    let (tx, rx) :(Sender<String>, Receiver<String>)= mpsc::channel();

    let handle = thread::spawn(move || {
        for i in 1..10 {
            tx.send(format!("item_{}",i)).unwrap()
        } 
    });


    for received in rx {
        println!("result:{}", received);
    }
}
