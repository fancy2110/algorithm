mod intern;

use std::ops::DerefMut;
use std::ops::Deref;
use std::ptr;
use std::mem;
use std::marker::PhantomData;
use std::option::Option;
use std::iter::Iterator;
use std::mem::{ManuallyDrop};
use crate::vec::intern::RawVec;
use crate::vec::intern::RawValIter;

/**
 *  自定义的Vec容器
*/
pub struct Vec<T> {
    len: usize, //当前容器的元素数量
    buf : RawVec<T>, 
}

/**
 *  用于直接遍历容器中的值，而不是通过引用
 */
pub struct IntoIter<T> {
    buf: RawVec<T>,
    iter: RawValIter<T>,
}

/***
 * 提供线程共享的能力
*/
unsafe impl<T:Send> Send for Vec<T> {}
unsafe impl<T:Sync> Sync for Vec<T> {}

impl<T> Vec<T> {
    /**
     * 构造一个初始容量为cap的容器
    */
    pub fn new() -> Self {
        Vec {
            len: 0,
            buf : RawVec::new(),
        }
    } 

    fn ptr(&mut self) -> *mut T {
        self.buf.ptr.as_ptr()
    }

    fn cap(&self) -> usize {
        self.buf.cap
    }

    /***
     * 加入一个元素
     */
    pub fn push (&mut self, elem: T) {
        //判断容量
        if self.cap() == self.len { self.buf.grow(); }

        unsafe {
            ptr::write(self.ptr().add(self.len), elem);
        }

        self.len += 1;
    }

    /**
     * 弹出尾部的元素
    */
    pub fn pop(&mut self) -> Option<T> {
        if self.len == 0 {
            None
        } else {
            self.len -= 1;
            unsafe {
                Some(ptr::read(self.ptr().add(self.len)))
            }
        }
    }
    
    /**
     * 返回容器的数据数量
    */
    pub fn len(&self) -> usize {
        self.len
    }

    /**
     * 往数组中添加元素, 在指定位置插入
    */
    pub fn insert(&mut self, index:usize, elem:T) {
        assert!(self.len >= index, "index out of bounds");

        if self.cap() == self.len { self.buf.grow() }

        unsafe {
            ptr::copy(
                self.ptr().add(index),
                self.ptr().add(index+1), 
                self.len - index);
            
            ptr::write(self.ptr().add(index), elem);
            self.len += 1;
        };
    }

    /**
     * 移除特定的元素
    */
    pub fn remove(&mut self, index:usize) -> T {
        assert!(index < self.len, "index out of bounds"); 

        unsafe {
            self.len -= 1;
            let result = ptr::read(self.ptr().add(index));

            ptr::copy(
                self.ptr().add(index+1), 
                self.ptr().add(index), 
                self.len - index); //之前len已经完成了-1了

            result
        }
    }

    pub fn last(&self) -> Option<&T>{
        if self.len == 0 {
            None
        } else {
            Some(&self[self.len - 1])
        }
    }

    pub fn into_iter(self) -> IntoIter<T> {
        let me = ManuallyDrop::new(self);
        println!("value iter");
        unsafe {
            let buf = ptr::read(&me.buf);

            IntoIter {
                iter: RawValIter::new(&me),
                buf : buf,
            }
        }
    }

    pub fn drain(&mut self) -> Drain<T> {
        unsafe {
            let iter = RawValIter::new(self);
            self.len = 0;

            Drain {
                vec :PhantomData,
                iter: iter
            }
        }
    }
}

/**处理上数据回收*/
impl<T> Drop for Vec<T> {

    fn drop(&mut self) { 
        println!("vec drop");
            while let Some(_) = self.pop() {}
    }
}

/**支持slice的作用*/
impl<T> Deref for Vec<T> {
    type Target = [T];
    
    fn deref(&self) -> &<Self as Deref>::Target { 
        unsafe {
            std::slice::from_raw_parts(self.buf.ptr.as_ptr(), self.len)
        }
    }
}

impl<T> DerefMut for Vec<T>{
    fn deref_mut(&mut self) -> &mut <Self as std::ops::Deref>::Target { 
        unsafe {
            std::slice::from_raw_parts_mut(self.ptr(), self.len)
        }
     }
}

/***
 * 处理按值遍历的相关逻辑
 * */
 impl<T> Iterator for IntoIter<T> {
    type Item = T;
    fn next(&mut self) -> Option<T> { 
        self.iter.next() 
    }

    fn size_hint(&self) -> (usize, Option<usize>) {
        self.iter.size_hint()
    }
 }

 impl<T> DoubleEndedIterator for IntoIter<T> {

    fn next_back(&mut self) -> Option<<Self as Iterator>::Item> { 
        self.iter.next_back()
    }
 }

 impl<T> Drop for IntoIter<T> {

    fn drop(&mut self) { 
        for _ in &mut *self { } //删除遗留的元素
    }
 }

 /***
  *  for Drain
 */
pub struct Drain<T> {
    vec : PhantomData<Vec<T>>,
    iter: RawValIter<T>,
}

impl<T> Iterator for Drain<T> {
    type Item = T;
    fn next(&mut self) -> Option<T> { self.iter.next() }
    fn size_hint(&self) -> (usize, Option<usize>) { self.iter.size_hint() }
}

impl<T> DoubleEndedIterator for Drain<T> {
    fn next_back(&mut self) -> Option<T> { self.iter.next_back() }
}

impl<T> Drop for Drain<T> {
    fn drop(&mut self) {
        for _ in &mut *self {}
    }
}
