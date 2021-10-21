use std::thread;
use std::time::Duration;
use std::sync::mpsc;
use std::sync::{Arc, Mutex};
use std::rc::Rc;
use std::sync::mpsc::{Sender, Receiver};

fn main() {
    test_mutex();
}

fn test_mutex() {
    let counter_ref = Arc::new(Mutex::new(0)); 

    let mut handles = vec![];

    for _ in 0..10 {
        let counter = Arc::clone(&counter_ref);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();
            *num += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap(); 
    }

    println!("reuslt:{}", *counter_ref.lock().unwrap())
}

fn test_channel() {
    let (tx, rx) :(Sender<String>, Receiver<String>)= mpsc::channel();

    let tx1 = tx.clone();
    let handle = thread::spawn(move || {
        for i in 1..10 {
            tx.send(format!("item_{}",i)).unwrap();
            thread::sleep(Duration::from_micros(10));
        } 
    });

    let handle2 = thread::spawn(move || {
        for i in 1..10 {
            tx1.send(format!("item_group1_{}", i));
            thread::sleep(Duration::from_micros(10));
        } 
    });


    for received in rx {
        println!("result:{}", received);
    }

    handle.join().unwrap();
    handle2.join().unwrap();
}

fn test_join() {
    let handle = thread::spawn(|| {
        for i in 1..10 {
            println!("thread index {}", i);
            thread::sleep(Duration::from_secs(1));
        }
    });

    for i in 1..10 {
        println!("main thread index{}", i); 
    }
    handle.join().unwrap()
}
