
-- Header

-- [[file:~/src/silveretta/doc/exercises/e1.1.org::*Header][Header:1]]

module E1 where

-- Header:1 ends here

-- Types
--    We defined the types for the Tree.

-- [[file:~/src/silveretta/doc/exercises/e1.1.org::*Types][Types:1]]

newtype Key = Key String deriving Eq
data Tree = Leaf 
          | Tree Tree Key Tree

-- Types:1 ends here

-- Defintions

-- [[file:~/src/silveretta/doc/exercises/e1.1.org::*Defintions][Defintions:1]]

empty :: Tree
empty = Leaf

insert :: Key -> Tree -> Tree
insert k Leaf = Tree Leaf k Leaf
insert k (Tree l v r) = 
  if k < v then
    Tree (insert k l) v r
  else if k > v then
    Tree l v (insert k r)
  else
    Tree l k r

-- Defintions:1 ends here

-- Part a

-- [[file:~/src/silveretta/doc/exercises/e1.1.org::*Part%20a][Part\ a:1]]

member :: Key -> Tree -> Bool
member _ Leaf = False
member k (Tree l v r)
  | k == v = True
  | k <  v = member k l
  | k >  v = member k r

-- Part\ a:1 ends here
