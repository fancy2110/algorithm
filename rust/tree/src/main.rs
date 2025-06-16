use std::{
    cell::RefCell,
    collections::{HashMap, VecDeque},
    fmt::{Debug, Display},
    hash::Hash,
    path::PathBuf,
    sync::Arc,
};

#[derive(Debug)]
struct Node<K, T>
where
    K: PartialEq + Hash + Clone,
{
    key: K,
    value: T,
    children: Vec<NodeRef<K, T>>,
    parent: Option<NodeRef<K, T>>,
}

type NodeRef<K, T> = Arc<RefCell<Node<K, T>>>;

impl<K, T> Node<K, T>
where
    K: PartialEq + Hash + Clone,
{
    fn new(key: K, value: T) -> Node<K, T> {
        Node {
            key,
            value,
            children: Vec::new(),
            parent: None,
        }
    }

    fn add_child(&mut self, child: Node<K, T>) -> NodeRef<K, T> {
        let node = Arc::new(RefCell::new(child));
        self.children.push(node.clone());
        node
    }

    /**
     *  remove all children from this node and return all nodes contains in this subtree
     */
    fn remove_child(&mut self, key: &K) -> Vec<NodeRef<K, T>> {
        let position = self
            .children
            .iter()
            .position(|item| item.borrow().key == *key);

        let mut elems: Vec<NodeRef<K, T>> = vec![];
        if let Some(position) = position {
            let elem = self.children.remove(position);
            let mut queue: VecDeque<NodeRef<K, T>> = VecDeque::new();
            queue.push_back(elem);
            while let Some(node) = queue.pop_front() {
                let children: Vec<NodeRef<K, T>> = node.borrow_mut().children.drain(0..).collect();
                queue.extend(children);
                elems.push(node.clone());
            }
        }

        elems
    }
}

// impl<K, T> Drop for Node<K, T>
// where
//     K: PartialEq + Hash + Clone,
// {
//     fn drop(&mut self) {
//     }
// }

impl<K, T> PartialEq for Node<K, T>
where
    K: PartialEq + Hash + Clone,
{
    fn eq(&self, other: &Self) -> bool {
        self.key == other.key
    }
}

#[derive(Debug)]
struct Tree<K, T>
where
    K: PartialEq + Hash + Clone,
{
    root: Option<NodeRef<K, T>>,
    nodes: HashMap<K, NodeRef<K, T>>,
}

impl<K, T> Tree<K, T>
where
    K: Eq + Hash + Clone,
{
    fn from_value(key: K, value: T) -> Tree<K, T> {
        let node = Node::new(key, value);
        Self::from_node(node)
    }

    fn from_node(node: Node<K, T>) -> Tree<K, T> {
        let key = node.key.clone();
        let node = Arc::new(RefCell::new(node));
        Tree {
            root: Some(node.clone()),
            nodes: HashMap::from([(key, node.clone())]),
        }
    }

    fn insert(&mut self, key: &K, value: Node<K, T>) -> Result<(), String> {
        let node_key = value.key.clone();
        let node = if let Some(parent) = self.nodes.get_mut(key) {
            let mut parent = parent.borrow_mut();
            Some(parent.add_child(value))
        } else {
            None
        };

        if let Some(node) = node {
            self.nodes.insert(node_key, node);
            Ok(())
        } else {
            Err(format!("node not found"))
        }
    }

    fn remove(&mut self, key: &K) -> Result<(), String> {
        let nodes: Vec<NodeRef<K, T>> = match self.nodes.remove(key) {
            Some(value) => {
                let parent = value.borrow_mut().parent.take();
                if let Some(node_ref) = parent {
                    let mut node = node_ref.borrow_mut();
                    node.remove_child(key)
                } else {
                    vec![]
                }
            }
            None => vec![],
        };

        /*
         * remove all cache node from search map
         */
        for node in nodes {
            self.nodes.remove(&node.borrow().key);
        }
        Ok(())
    }

    fn get(&self, key: &K) -> Option<NodeRef<K, T>> {
        return self.nodes.get(key).cloned();
    }

    fn size(&self) -> usize {
        self.nodes.len()
    }
}

#[derive(Debug)]
struct File {
    path: PathBuf,
    name: String,
}

impl File {
    fn new(path: PathBuf, name: &str) -> File {
        File {
            path: path,
            name: name.to_string(),
        }
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
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    fn create_tree() -> Tree<PathBuf, File> {
        let key = PathBuf::from("/");
        let value = File::new(key.clone(), "/");
        let node = Node::new(key.clone(), value);
        Tree::from_node(node)
    }

    #[test]
    fn test_tree_size_growth() {
        let mut tree: Tree<PathBuf, File> = create_tree();
        let key = PathBuf::from("a.txt");
        let value = File::new(key.clone(), "a.txt");
        let node = Node::new(key.clone(), value);
        if let Ok(_) = tree.insert(&PathBuf::from("/"), node) {
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
        let _ = tree.insert(&PathBuf::from("/"), node);
        if let Some(node) = tree.get(&key) {
            assert_eq!(node.borrow().key, key);
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
        let _ = tree.insert(&PathBuf::from("/"), node);
        if let Some(node) = tree.get(&key) {
            let ret = tree.remove(&key);
            assert!(ret.is_ok());
            assert_eq!(tree.size(), 1);

            if let Some(node) = tree.get(&key) {
                assert!(false);
            } else {
                assert!(true);
            }
        } else {
            assert!(false)
        }
    }
}
