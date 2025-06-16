pub struct Node<T> {
    next: Option<Box<Node<T>>>,
    value: T,
}

pub struct Link<T> {
    head: Option<Box<Node<T>>>,
    count: u32,
}

impl<T> Link<T> {
    pub fn new() -> Self {
        Link {
            head: None,
            count: 0,
        }
    }

    pub fn peek(&self) -> Option<&T> {
        self.head.as_ref().map(|node| &node.value)
    }

    pub fn peek_mut(&mut self) -> Option<&mut T> {
        self.head.as_mut().map(|node| &mut node.value)
    }

    pub fn push(&mut self, elem: T) -> bool {
        self.head = Some(Box::new(Node {
            next: self.head.take(),
            value: elem,
        }));
        self.count += 1;
        false
    }

    pub fn pop(&mut self) -> Option<T> {
        self.head.take().map(|node| {
            self.head = node.next;
            node.value
        })
    }
}

impl<T> Drop for Link<T> {
    fn drop(&mut self) {
        let mut tmp = self.head.take();
        while let Some(mut tmp1) = tmp {
            tmp = tmp1.next.take();
        }
    }
}

pub struct LinkMutIter<'a, T: 'a> {
    cur: Option<&'a mut Node<T>>,
}

impl<'a, T> LinkMutIter<'a, T> {
    fn new(cur: Option<&'a mut Node<T>>) -> LinkMutIter<'a, T> {
        LinkMutIter { cur: cur }
    }
}

pub struct LinkIter<'a, T> {
    cur: &'a Option<Box<Node<T>>>,
}

pub struct LinkIntoIter<T> {
    link: Link<T>,
}

impl<T> Iterator for LinkIntoIter<T> {
    type Item = T;

    fn next(&mut self) -> Option<Self::Item> {
        self.link.pop()
    }
}

impl<'a, T> Iterator for LinkIter<'a, T> {
    type Item = &'a T;

    fn next(&mut self) -> Option<Self::Item> {
        if let Some(cur) = self.cur {
            let value = Some(&cur.value);
            self.cur = &cur.next;
            value
        } else {
            None
        }
    }
}

impl<'a, T> Iterator for LinkMutIter<'a, T> {
    type Item = &'a mut T;

    fn next(&mut self) -> Option<Self::Item> {
        self.cur.take().map(|node| {
            self.cur = node.next.as_deref_mut();
            &mut node.value
        })
    }
}

impl<T> IntoIterator for Link<T> {
    type Item = T;

    type IntoIter = LinkIntoIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        LinkIntoIter { link: self }
    }
}

impl<'a, T> IntoIterator for &'a Link<T> {
    type Item = &'a T;

    type IntoIter = LinkIter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        LinkIter { cur: &self.head }
    }
}

impl<'a, T> IntoIterator for &'a mut Link<T> {
    type Item = &'a mut T;

    type IntoIter = LinkMutIter<'a, T>;

    fn into_iter(self) -> Self::IntoIter {
        LinkMutIter::new(self.head.as_deref_mut())
    }
}

// impl<T> Iterator for Link<T> {
//     type Item = &T;

//     fn next(&mut self) -> Option<Self::Item> {
//         todo!()
//     }
// }
