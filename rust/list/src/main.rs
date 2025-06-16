use list::list::Link;

fn main() {
    println!("Hello, world!");
    let mut link: Link<i32> = Link::new();
    link.push(1);

    // let value = link.pop();
    // println!("value :{:?}", value);

    for item in &mut link {
        println!("value :{:?}", *item);
        *item += 2;
    }

    for item in &link {
        println!("value :{:?}", *item);
    }

    let items = vec![0, 1, 2, 3, 4];
    // for item in &items {}

    print!("items:{:?}", link.peek())
}
