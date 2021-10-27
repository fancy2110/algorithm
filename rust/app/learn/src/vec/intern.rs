use std::ptr::{self, NonNull};
use std::marker::PhantomData;
use std::alloc::{self, Layout};
use std::mem;


/**
 * 优化vec的实现逻辑
 * **/
pub struct RawVec<T> {
    pub ptr :NonNull<T>,
    pub cap :usize,
    _marker : PhantomData<T>,
}

unsafe impl <T: Sync> Sync for RawVec<T> {}
unsafe impl <T: Send> Send for RawVec<T> {}

impl<T> RawVec<T> {
    pub fn new() -> Self {
        assert!(mem::size_of::<T>() != 0, "TODO: implements ZST support");
        RawVec {
            ptr : NonNull::dangling(),
            cap : 0,
            _marker : PhantomData,
        }
    } 

    pub fn grow(&mut self) {
        let (new_cap, new_layout) = if self.cap == 0 {
            (1, Layout::array::<T>(1).unwrap())
        } else {
            /* 容量扩展两倍 */
            let new_cap = 2 * self.cap;
            //调整布局
            let new_layout = Layout::array::<T>(new_cap).unwrap();
            (new_cap, new_layout)
        };

        let new_ptr = if self.cap == 0 {
            //从未分配过内存    
            unsafe {alloc::alloc(new_layout)}
        } else {
            let old_layout = Layout::array::<T>(self.cap).unwrap();
            let old_ptr = self.ptr.as_ptr() as *mut u8;
            unsafe { alloc::realloc(old_ptr, old_layout, new_layout.size())}
        };

        self.ptr = match NonNull::new(new_ptr as *mut T) {
            Some(p) => p, 
            None => alloc::handle_alloc_error(new_layout),
        };

        self.cap = new_cap;
    }
}

impl<T> Drop for RawVec<T> {
    fn drop(&mut self) {
        if self.cap == 0 { return ; }
        let layout = Layout::array::<T>(self.cap).unwrap();
        unsafe {
            alloc::dealloc(self.ptr.as_ptr() as *mut u8, layout);
        }
    }
}


/************** Drain for vec*****************/
pub struct RawValIter<T> {
    start : *const T,
    end : *const T,
}

impl<T> RawValIter<T> {
    pub unsafe fn new(slice : &[T]) -> Self {
        RawValIter {
            start : slice.as_ptr(),
            end: if slice.len() == 0 {
                slice.as_ptr()
            } else {
                slice.as_ptr().add(slice.len())
            }
        }
    }
}

impl<T> Iterator for RawValIter<T> {
    type Item = T;
    fn next(&mut self) -> Option<T> { 
        if self.start == self.end {
            None
        } else {
            unsafe{
                let result = ptr::read(self.start);
                self.start = self.start.offset(1);
                Some(result)
            }
        }
    }

    fn size_hint(&self) -> (usize, Option<usize>) {
        let len = (self.end as usize - self.start as usize) / mem::size_of::<T>();
        (len, Some(len))
    }
}

impl<T> DoubleEndedIterator for RawValIter<T> {

    fn next_back(&mut self) -> Option<<Self as Iterator>::Item> { 
        if self.end == self.start {
            None
        } else {
            unsafe {
                self.end = self.end.offset(-1);
                let result = ptr::read(self.end);
                Some(result)
            }
        }
    }
}
