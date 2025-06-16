use core::str;
use std::rc::Rc;

pub struct Node<T> {
    elem: T,
    next: Link<T>,
}

type Link<T> = Option<Rc<Node<T>>>;

pub struct List<T> {
    head: Link<T>,
}

impl<T> List<T> {
    pub fn new() -> List<T> {
        List { head: None }
    }

    /**
     * list = B -> C -> D
     *
     * return
     * list = prepend(list, A) = A -> B -> C -> D
     * **/
    pub fn prepend(&self, elem: T) -> List<T> {
        List {
            head: Some(Rc::new(Node {
                elem,
                next: self.head.clone(),
            })),
        }
    }

    /**
     * list = B -> C -> D
     *
     * return
     * list = tail(list) = C -> D
     * **/
    pub fn tail(&self) -> List<T> {
        let head = self.head.as_ref();
        List {
            head: head.and_then(|node| node.next.clone()),
        }
    }

    pub fn head(&self) -> Option<&T> {
        if let Some(head) = &self.head {
            Some(&head.as_ref().elem)
        } else {
            None
        }
    }
}

impl<T> Drop for List<T> {
    fn drop(&mut self) {
        let mut next = self.head.take();
        while let Some(item) = next {
            if let Ok(mut node) = Rc::try_unwrap(item) {
                next = node.next.take();
            } else {
                break;
            }
        }
    }
}

pub struct RefIter<'a, T> {
    next: Option<&'a Rc<Node<T>>>,
}

impl<'a, T> Iterator for RefIter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        self.next.take().and_then(|node| {
            self.next = node.next.as_ref();
            Some(&node.elem)
        })
    }
}

impl<'a, T> IntoIterator for &'a List<T> {
    type Item = &'a T;

    type IntoIter = RefIter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        RefIter {
            next: self.head.as_ref(),
        }
    }
}
