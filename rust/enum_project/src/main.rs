use std::{cell::RefCell, rc::Rc};

pub mod types;

enum LinkNode {
   Int(Rc<RefCell<Box<Item1>>>) ,
   Boolean(Rc<RefCell<Box<Item2>>>)
}

struct Link {
    pub data: LinkNode,
    pub next: Option<Box<Link>>
}
impl Link {
    pub fn add(&self, item: Link) {
        
    } 
}

struct Item1 {
    pub link: Option<Box<Link>>,
    pub value: i32,
}

struct Item2 {
    pub link: Option<Box<Link>>,
    pub value: bool
}


fn main() {
    let a = Box::new(Item1 {
        link: None,
        value: 10,
    });
    let a = Rc::new(RefCell::new(a));
    let alink = Link{
        data: LinkNode::Int(a.clone()),
        next: None,
    };
    // let a = a.clone();
    let mut a = a.borrow_mut();
    a.link = Some(Box::new(alink));


    let b: Box<Item2> = Box::new(Item2 {
        link: None,
        value: false,
    });
    let b = Rc::new(RefCell::new(b));
    let blink = Box::new(Link{
        data: LinkNode::Boolean(b.clone()),
        next: None,
    });
    // let a = a.clone();
    let mut b = b.borrow_mut();
    b.link = Some(blink);
    // if let Some(link) = &mut b.link  {
    //     link.add(alink);
    // }

}

