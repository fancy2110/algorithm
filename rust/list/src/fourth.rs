use std::{
    borrow::BorrowMut,
    cell::{Ref, RefCell, RefMut},
    rc::Rc,
};

pub struct Node<T> {
    elem: T,
    next: Link<T>,
    prev: Link<T>,
}

type Link<T> = Option<Rc<RefCell<Node<T>>>>;

pub struct List<T> {
    head: Link<T>,
    tail: Link<T>,
}

impl<T> List<T> {
    pub fn new() -> List<T> {
        List {
            head: None,
            tail: None,
        }
    }

    pub fn push_front(&mut self, elem: T) {
        let node = Rc::new(RefCell::new(Node {
            elem,
            next: None,
            prev: None,
        }));

        match self.head.take() {
            Some(head) => {
                head.as_ref().borrow_mut().prev = Some(node.clone());
                node.as_ref().borrow_mut().next = Some(head);
                self.head = Some(node)
            }

            None => {
                self.head = Some(node.clone());
                self.tail = Some(node);
            }
        }
    }

    pub fn push_back(&mut self, elem: T) {
        let node = Rc::new(RefCell::new(Node {
            elem: elem,
            next: None,
            prev: None,
        }));

        match self.tail.take() {
            Some(tail) => {
                tail.as_ref().borrow_mut().next = Some(node.clone());
                node.as_ref().borrow_mut().prev = Some(tail);
                self.tail = Some(node);
            }

            None => {
                self.head = Some(node.clone());
                self.tail = Some(node.clone());
            }
        }
    }

    pub fn pop_front(&mut self) -> Option<T> {
        match self.head.take() {
            Some(head) => {
                {
                    let mut node = head.as_ref().borrow_mut();
                    if let Some(next) = node.next.take() {
                        next.as_ref().borrow_mut().prev = None;
                        self.head = Some(next);
                    } else {
                        self.head = None;
                        self.tail = None;
                    }
                }

                Some(Rc::try_unwrap(head).ok().unwrap().into_inner().elem)
            }

            None => None,
        }
    }

    pub fn pop_back(&mut self) -> Option<T> {
        match self.tail.take() {
            Some(tail) => {
                let pre_tail = tail.as_ref().borrow_mut().prev.take();
                if let Some(pre) = pre_tail {
                    pre.as_ref().borrow_mut().next = None;
                    self.tail = Some(pre);
                } else {
                    self.head = None;
                    self.tail = None;
                }

                Some(Rc::try_unwrap(tail).ok().unwrap().into_inner().elem)
            }
            None => None,
        }
    }

    pub fn peek_front(&self) -> Option<Ref<T>> {
        self.head
            .as_ref()
            .map(|node| Ref::map(node.borrow(), |node| &node.elem))
    }

    pub fn peek_back(&self) -> Option<Ref<T>> {
        self.tail
            .as_ref()
            .map(|node| Ref::map(node.borrow(), |node| &node.elem))
    }

    pub fn peek_front_mut(&mut self) -> Option<RefMut<T>> {
        self.head.as_ref().map(|node| {
            let new_node = node.as_ref().borrow_mut();
            RefMut::map(new_node, |node| &mut node.elem)
        })
    }

    pub fn peek_back_mut(&mut self) -> Option<RefMut<T>> {
        self.tail.as_ref().map(|node| {
            let new_node = node.as_ref().borrow_mut();
            RefMut::map(new_node, |node| &mut node.elem)
        })
    }
}

impl<T> Drop for List<T> {
    fn drop(&mut self) {
        while self.pop_front().is_some() {}
    }
}

pub struct ListIter<T> {
    head: Link<T>,
    tail: Link<T>,
}

impl<T> Iterator for ListIter<T> {
    type Item = T;

    fn next(&mut self) -> Option<Self::Item> {
        self.head.as_ref().borrow_mut();
        None
    }
}

impl<T> DoubleEndedIterator for ListIter<T> {
    fn next_back(&mut self) -> Option<Self::Item> {
        None
    }
}

impl<T> IntoIterator for List<T> {
    type Item = T;

    type IntoIter = ListIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        ListIter {
            head: self.head.clone(),
            tail: self.tail.clone(),
        }
    }
}
