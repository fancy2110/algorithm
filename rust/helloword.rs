use std::ops::Deref;
use std::fmt::Display;
use std::collections::HashMap;
use std::hash::Hash;


struct MyBox<T:Display>(T);

#[derive(Debug)]
struct X<'a>(&'a i32);

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

impl Drop for X<'_> {
    fn drop(&mut self) {
        
    }
}
fn test_destruct_borrow_rule() {
    let mut data = vec![1,2,3,4,5];
    let mut x = X(&data[0]);
    Vec::new()
    
    println!("x is {:?}", x);
    drop(x);
    data.push(6);
}

fn as_str<'a>(data : &'a u32) -> String {
    let s = format!("{}", data);
    return s;
}

// lifetime test 
#[derive(Debug)]
struct Foo;
impl Foo {
    fn mute_and_share(&mut self) -> &Self { &*self }
    fn share(&self) {}
}

fn test_foo_borrow_rule() {
    let mut foo = Foo;
    let loan = foo.mute_and_share();
    foo.share();
    println!("foo:{:?}", foo);
}
/*******************************/

/***borrow test for map **/
#[derive(Debug)]
struct Item {
    id :i32,
}

impl Item {
    fn new(id: i32) -> Item {
        Item { id }
    }
}

impl Default for Item {
    fn default() -> Item {
        Item::new(0)
    }
}


fn get_default<'m, K, V> (data: &'m mut HashMap<K,V>, key:K) -> &'m mut V
    where 
        K: Clone + Eq + Hash,
        V: Default
{
    match data.get(&key){
        Some(value) => value,
        None => {
            let item = V::default();
            let a = &item;
            data.insert(key.clone(), item).unwrap();
        }
    }

    return data.get_mut(&key).unwrap();
}

fn test_get_default() {
    let mut book_reviews = HashMap::new();
    book_reviews.insert("book1", Item::new(1));
    book_reviews.insert("book2", Item::new(2));
    book_reviews.insert("book3", Item::new(3));

    let value = get_default(&mut book_reviews, "book4");
    println!("value {:?}", value);
}

fn main() {
    test_get_default();
}
