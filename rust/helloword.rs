use std::ops::Deref;
use std::fmt::Display;

struct MyBox<T:Display>(T);

impl <T: Display> MyBox<T> {
    fn new(x:T) -> MyBox<T> {
        MyBox(x)
    } 
}

impl <T: Display> Deref for MyBox<T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl <T: Display> Drop for MyBox<T> {
    fn drop(&mut self) {
        println!("box {} is droped", self.0);
    }
}


fn main() {
    let x = 5;
    let y = MyBox::new(x);
    assert_eq!(5 , x);
    assert_eq!(5 , *y);
}
