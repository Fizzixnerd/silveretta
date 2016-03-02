module SLPI where

newtype Id = Id String deriving Eq
newtype Numb = Numb Int deriving Eq

data Binop = Add | Sub | Mul | Div                                  

data Stm = CompoundStm Stm Stm
         | AssignmentStm Id Exp
         | PrintStm ExpList

data Exp = IdExp Id
         | NumExp Numb
         | BinopExp Exp Binop Exp
         | EseqExp Stm Exp

data ExpList = PairExpList Exp ExpList
             | LastExpList Exp

newtype Table = Table [(Id, Numb)]

lenExpList :: ExpList -> Int
lenExpList (PairExpList _ el) = 1 + (lenExpList el)
lenExpList (LastExpList _) = 1

maxArgsExp :: Exp -> Int
maxArgsExp (IdExp _) = 0
maxArgsExp (NumExp _) = 0
maxArgsExp (BinopExp e _ f) = max (maxArgsExp e) (maxArgsExp f)
maxArgsExp (EseqExp s e) = max (maxArgs s) (maxArgsExp e)

maxArgs :: Stm -> Int
maxArgs (CompoundStm s t) = max (maxArgs s) (maxArgs t)
maxArgs (AssignmentStm _ e) = maxArgsExp e
maxArgs (PrintStm el) = lenExpList el

emptyT :: Table
emptyT = Table []

updateT :: Table -> Id -> Numb -> Table
updateT (Table t) i n = Table ((i, n):t)

lookupT :: Table -> Id -> Numb
lookupT (Table []) _ = error "Could not find identifier."
lookupT (Table ((i, n):t)) l
 | i == l = n
 | otherwise = lookupT (Table t) l

interpStm :: Stm -> Table -> Table                              
interpStm (CompoundStm s1 s2) t = interpStm s2 (interpStm s1 t)
interpStm (AssignmentStm i e) t = updateT u i n
  where
    (n, u) = interpExp e t
interpStm (PrintStm el) t = interpExpList el t

interpExp :: Exp -> Table -> (Numb, Table)                      
interpExp (IdExp i) t = (lookupT t i, t)
interpExp (NumExp n) t = (n, t)
interpExp (BinopExp e1 bo e2) t = (evalBinop n bo m, v)
  where 
    (n, u) = interpExp e1 t
    (m, v) = interpExp e2 u
interpExp (EseqExp s e) t = interpExp e u
  where 
    u = interpStm s t

interpExpList :: ExpList -> Table -> Table
interpExpList (PairExpList e el) t = interpExpList el u
  where
    u = snd $ interpExp e t
interpExpList (LastExpList e) t = snd $ interpExp e t

evalBinop :: Numb -> Binop -> Numb -> Numb
evalBinop (Numb n) Add (Numb m) = Numb $ n + m
evalBinop (Numb n) Sub (Numb m) = Numb $ n - m
evalBinop (Numb n) Mul (Numb m) = Numb $ n * m
evalBinop (Numb n) Div (Numb m) = Numb $ n `div` m
