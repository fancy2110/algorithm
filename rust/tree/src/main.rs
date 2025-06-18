use std::{fmt::Debug, path::PathBuf};

use crate::tree::Tree;
mod node;
mod tree;

#[derive(Debug)]
struct File {
    path: PathBuf,
    name: String,
    pub size: usize,
}

impl File {
    fn new_with_size(path: PathBuf, name: &str, size: usize) -> File {
        File {
            path: path,
            name: name.to_string(),
            size: size,
        }
    }

    fn new(path: PathBuf, name: &str) -> File {
        File {
            path: path,
            name: name.to_string(),
            size: 0,
        }
    }
}

impl Drop for File {
    fn drop(&mut self) {
        println!("file : {:?}", self.name);
    }
}

fn main() {
    let tree: Tree<PathBuf, File> =
        Tree::from_value(PathBuf::from("/"), File::new(PathBuf::from("/"), "/"));

    println!("tree size: {:?}", tree.size());
    println!("tree root: {:?}", tree.root);
}

#[cfg(test)]
mod tests {
    use std::env::var;

    use crate::node::{Node, UpdateMode};

    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    fn create_tree() -> Tree<PathBuf, File> {
        let key = PathBuf::from("/");
        let value = File::new(key.clone(), "/");
        let node = Node::new(key.clone(), value);
        Tree::from_node(node)
    }

    fn create_multi_layer_tree() -> Tree<PathBuf, File> {
        let mut tree = create_tree();
        let var_dir = PathBuf::from("/var");
        let _ = tree.insert(
            &PathBuf::from("/"),
            var_dir.clone(),
            File::new(var_dir.clone(), "/var"),
        );

        for name in vec!["text1", "text2", "text3", "text4"] {
            let file_key = var_dir.clone().join(name);
            let value = File::new(file_key.clone(), name);
            let _ = tree.insert(&var_dir, file_key, value);
        }
        tree
    }

    #[test]
    fn test_tree_size_growth() {
        let mut tree: Tree<PathBuf, File> = create_tree();
        let key = PathBuf::from("a.txt");
        let value = File::new(key.clone(), "a.txt");
        let node = Node::new(key.clone(), value);
        if let Ok(_) = tree.insert_node(&PathBuf::from("/"), node) {
            assert_eq!(tree.size(), 2);
        } else {
            assert!(false);
        }
    }

    #[test]
    fn test_tree_insert_node() {
        let mut tree: Tree<PathBuf, File> = create_tree();
        let key = PathBuf::from("a.txt");
        let value = File::new(key.clone(), "a.txt");
        let node = Node::new(key.clone(), value);
        let _ = tree.insert_node(&PathBuf::from("/"), node);
        if let Some(node) = tree.get_node(&key) {
            assert_eq!(node.borrow().key, key);
        } else {
            assert!(false);
        }
    }

    #[test]
    fn test_tree_delete_nodes() {
        let var_dir = PathBuf::from("/var");
        let mut tree: Tree<PathBuf, File> = create_multi_layer_tree();
        println!("total {}", tree.size());
        if let Ok(sub_nodes) = tree.remove(&var_dir) {
            assert_eq!(sub_nodes.len(), 5);
        } else {
            assert!(false);
        }
    }

    #[test]
    fn test_tree_insert_nodes() {
        let var_dir = PathBuf::from("/var");
        let tree: Tree<PathBuf, File> = create_multi_layer_tree();
        if let Some(node) = tree.get_node(&var_dir) {
            assert_eq!(node.borrow().key, var_dir);
        } else {
            assert!(false);
        }
    }

    #[test]
    fn test_tree_remove_node() {
        let mut tree: Tree<PathBuf, File> = create_tree();
        let key = PathBuf::from("a.txt");
        let value = File::new(key.clone(), "a.txt");
        let node = Node::new(key.clone(), value);
        let _ = tree.insert_node(&PathBuf::from("/"), node);
        if let Some(node) = tree.get_node(&key) {
            let ret = tree.remove(&key);
            assert!(ret.is_ok());
            assert_eq!(tree.size(), 1);

            if let Some(node) = tree.get_node(&key) {
                assert!(false);
            } else {
                assert!(true);
            }
        } else {
            assert!(false)
        }
    }

    #[test]
    fn test_tree_update_node() {
        let mut tree: Tree<PathBuf, File> = create_tree();

        let mut parent_key = PathBuf::from("/");
        for path in vec!["var", "log", "app", "tmp", "cache", "log.txt"] {
            let key = PathBuf::from(path);
            let value = File::new_with_size(key.clone(), path, 10);
            let node = Node::new(key.clone(), value);
            if let Ok(node) = tree.insert_node(&parent_key, node) {
                let new_value = node.borrow().get_value().size;
                let mut item = node.borrow().get_parent();
                while let Some(parent) = item {
                    let mut value = parent.borrow_mut();
                    value.update(|node| node.size += new_value);
                    item = value.get_parent();
                }
            }
            parent_key = key;
        }

        if let Some(root) = &tree.root {
            let root_size = root.borrow().get_value().size;
            assert_eq!(root_size, 60);
        } else {
            assert!(false);
        }
    }
}
