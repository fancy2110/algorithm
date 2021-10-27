use learn::vec::Vec;
use std::mem::{self};

fn main() {
    let mut items:Vec<usize> = Vec::new();
    for i in 0..20 {
       items.push(i) 
    }


    let slice_items:&[usize] = &items[..];
    for elem in slice_items {
        println!("item:{}", elem);
    }

    println!("before size:{}", items.len());
    println!("origin last:{}", items.pop().unwrap());
    items.remove(18);
    items.insert(0, 100);

    let result = items.last();
    println!("modify last:{}", result.unwrap());

    items.remove(18);
    println!("after size:{}", items.len());

    for elem in items.drain() {
        println!("iter by drain : {}", elem);
    }

    println!("after size:{}", items.len());
}
