use std::{ alloc::{GlobalAlloc, System}, ptr::null_mut};
use std::sync::atomic::{
    AtomicUsize,
    Ordering::{Acquire, SeqCst},
};

#[derive(Debug)]
pub struct JSAllocator {
    malloc_count: AtomicUsize,
    malloc_size: AtomicUsize,
    malloc_limit: usize,
    enable_trace : bool,
    //void &opaque // 
}

impl JSAllocator {
    const MAX_SUPPORTED_ALIGN: usize = 4096;

    pub fn set_memory_limit(&mut self, limit: usize) {
        self.malloc_limit = limit
    }

    pub fn set_enable_trace(&mut self, enable: bool) {
        self.enable_trace = enable
    }

    pub fn log(&self) {
        println!("count:{}", self.malloc_count.fetch_add(0, SeqCst));
        println!("size:{}", self.malloc_size.fetch_add(0, SeqCst));
        println!("limit:{}", self.malloc_limit);
        println!("enable_trace:{}", self.enable_trace);
    }
}


unsafe impl GlobalAlloc for JSAllocator {

    unsafe fn alloc(&self, layout: std::alloc::Layout) -> *mut u8 {
       let size = layout.size();
        let align = layout.align();

        // `Layout` contract forbids making a `Layout` with align=0, or align not power of 2.
        // So we can safely use a mask to ensure alignment without worrying about UB.
        let align_mask_to_round_down = !(align - 1);
        
        if align > JSAllocator::MAX_SUPPORTED_ALIGN {
            return null_mut() 
        }

        let mut allocated = 0;

        let new_malloc_size = self.malloc_size.fetch_add(0 , SeqCst) + size;
        if new_malloc_size > self.malloc_limit {
            return null_mut()
        }

        let new_ptr = System.alloc(layout);
        if new_ptr.is_null() {
            return null_mut()
        }

        
        //todo add print log for debug

        self.malloc_count.fetch_add(1, SeqCst);
        self.malloc_size.fetch_add(size, SeqCst);

        return new_ptr
    }

    unsafe fn dealloc(&self, ptr: *mut u8, layout: std::alloc::Layout) {
        if ptr.is_null() {
            return;
        }

        self.malloc_count.fetch_sub(1, SeqCst);
        self.malloc_size.fetch_sub(layout.size(), SeqCst);
        //add print log for debug
        System.dealloc(ptr, layout);
    }

    unsafe fn alloc_zeroed(&self, layout: std::alloc::Layout) -> *mut u8 {
        let size = layout.size();
        // SAFETY: the safety contract for `alloc` must be upheld by the caller.
        let ptr = unsafe { self.alloc(layout) };
        if !ptr.is_null() {
            // SAFETY: as allocation succeeded, the region from `ptr`
            // of size `size` is guaranteed to be valid for writes.
            unsafe { std::ptr::write_bytes(ptr, 0, size) };
        }
        ptr
    }

    unsafe fn realloc(&self, ptr: *mut u8, layout: std::alloc::Layout, new_size: usize) -> *mut u8 {
        // SAFETY: the caller must ensure that the `new_size` does not overflow.
        // `layout.align()` comes from a `Layout` and is thus guaranteed to be valid.
        let new_layout = unsafe { std::alloc::Layout::from_size_align_unchecked(new_size, layout.align()) };
        // SAFETY: the caller must ensure that `new_layout` is greater than zero.
        let new_ptr = unsafe { self.alloc(new_layout) };
        if !new_ptr.is_null() {
            // SAFETY: the previously allocated block cannot overlap the newly allocated block.
            // The safety contract for `dealloc` must be upheld by the caller.
            unsafe {
                std::ptr::copy_nonoverlapping(ptr, new_ptr, std::cmp::min(layout.size(), new_size));
                self.dealloc(ptr, layout);
            }
        }
        new_ptr
    }

}

#[global_allocator]
static mut ALLOCATOR: JSAllocator = JSAllocator {
    malloc_count: AtomicUsize::new(0), 
    malloc_size: AtomicUsize::new(0), 
    malloc_limit: 100000,
    enable_trace: false, 
};


fn main() {
    let mut str = String::from("Hello, world!");
    let first = first_word(&str);
    println!("{}", first);

    unsafe { ALLOCATOR.log() };

    str.clear();

    unsafe { ALLOCATOR.log() };
}

fn first_word(s: &String) -> &str {
    let bytes = s.as_bytes();

    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return &s[0..i];
        }
    }

    return &s[..];
}
