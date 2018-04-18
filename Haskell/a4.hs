
-- a4 = 1

len :: [a] -> Int
len []     = 0
len (x:xs) = 1 + len xs

snoc :: a -> [a] -> [a]
snoc a []     = [a]
snoc a (x:xs) = x : (snoc a xs)

myappend :: [a] -> [a] -> [a]
myappend [] b     = b
myappend a []     = a
myappend (x:xs) b = x : (myappend xs b)


--cs.au.dk/~gerth/fds98/reverse.html
myreverse :: [a] -> [a]
myreverse []      = []
myreverse (x: xs) = myappend (myreverse xs) [x]

smallest_divisor :: Int -> Int
smallest_divisor n
   | n < 0       = error "n must be >= 0"
   | n == 0      = 0
   | n == 1      = 1
   | otherwise   = head (dropWhile (\x -> n `mod` x /= 0) [2..n])

is_prime :: Int -> Bool
is_prime n | n < 2     = False
           | otherwise = (smallest_divisor n) == n

is_composite :: Int -> Bool
is_composite n | n <= 1     = False
               | otherwise  = not (is_prime n)

reverseNum :: Int -> Int
reverseNum a = read $ myreverse $ show a

--example: length(filter (\x -> (is_prime $ reverseNum x) && is_prime x && (reverseNum x /=x)) [1..200])
emirp_list :: Int -> [Int] 
emirp_list n = (filter (\x -> (is_prime $ reverseNum x) && is_prime x && (reverseNum x /=x)) [1..(n-1)])

count_emirps :: Int -> Int
count_emirps n = length $ emirp_list n


maxVal :: [Int] -> Int
maxVal [x] = x
maxVal (x:xs) | (maxVal xs) > x = maxVal xs
              | otherwise       = x

-- biggest_sum :: [[Int]] -> [Int]
-- [[1,2],[3,5],[7,20]]
sum_int :: [Int] -> Int
sum_int []     = 0
sum_int (x:xs) = x + sum_int xs

biggest_sum :: [[Int]] -> [Int]
biggest_sum []   = []
biggest_sum [[]] = []
biggest_sum a = head(filter (\x -> (sum_int x) == (maxVal $ map sum a)) a)

--similar to my maxVal function

greatest :: (a -> Int) -> [a] -> a
greatest f [x] = x
greatest f (x:xs)
             | f x > f (greatest f xs) = x
             | otherwise                 = (greatest f xs)

is_bit :: (Num a, Eq a) => a -> Bool
is_bit a
       | a == 0 || a == 1 = True
       | otherwise        = False

flip_bit :: (Num a, Eq a) => a -> Int
flip_bit a 
       | a == 0 = 1
       | a == 1 = 0
       | otherwise = error "Not a bit"

is_bit_seq1 :: (Num a, Eq a) => [a] -> Bool
is_bit_seq1 [] = True
is_bit_seq1 (x:xs)
            | is_bit x == is_bit_seq1 xs = True
            | otherwise                  = False

is_bit_seq2 :: (Num a, Eq a) => [a] -> Bool
is_bit_seq2 [] = True
is_bit_seq2 (x:xs) =
                   if is_bit x == is_bit_seq2 xs
                   then True
                   else False

is_bit_seq3 :: (Num a, Eq a) => [a] -> Bool
is_bit_seq3 x = all is_bit x

invert_bits1 ::(Num a, Eq a) => [a] -> [Int]
invert_bits1 []     = []
invert_bits1 (x:xs) = flip_bit x : (invert_bits1 xs)

invert_bits2 :: (Num a, Eq a) => [a] -> [Int]
invert_bits2 x = map flip_bit x

invert_bits3 :: (Num a, Eq a) => [a] -> [Int]
invert_bits3 x = [flip_bit a | a <- x]

count_general :: Eq a => a -> [a] -> Int
count_general val list = length (filter (== val) list)

bit_count :: (Eq a, Num a) => [a] -> (Int, Int)
bit_count [] = (0,0)
bit_count x 
          | is_bit_seq3 x = (count_general 0 x, count_general 1 x)
          | otherwise     = error "Not a bit sequence"



--math help https://www.wikihow.com/Convert-from-Decimal-to-Binary
convert_to_binary :: (Num a2, Integral a1) => a1 -> [a2]
convert_to_binary 0 = [0]
convert_to_binary x
                  | x `mod` 2 == 1 = convert_to_binary (x `div` 2) ++ [1]
                  | x `mod` 2 == 0 = convert_to_binary (x `div` 2) ++ [0]

bit_seq_helper :: (Num a2, Integral a1) => a1 -> [[a2]]
bit_seq_helper x = map convert_to_binary [0..x]

check_length :: Foldable t => Int -> t a -> Bool
check_length n lst1
             | (length lst1) < n = True
             | otherwise         = False

-- Num a => Int -> [[a]]
all_basic_bit_seqs :: Num a => Int -> [[a]]
all_basic_bit_seqs 0 = []
all_basic_bit_seqs 1 = [[0],[1]]
all_basic_bit_seqs 2 = [[0,0],[0,1],[1,0]]
all_basic_bit_seqs n = map (\x -> if check_length n x then (replicate (n - (length x)) 0) ++ x else x) (bit_seq_helper n)


data List a = Empty | Cons a (List a)
    deriving Show

toList :: [a] -> List a
toList []     = Empty
toList [x]    = Cons x Empty
toList (x:xs) = Cons x (toList xs)

-- (Cons 1 (Cons 1 (Cons 2 (Cons 3 Empty))))


toHaskellList :: List a -> [a]
toHaskellList Empty          = []
toHaskellList (Cons x Empty) = [x]
toHaskellList (Cons x xs)    = [x] ++ (toHaskellList xs)

append:: List a -> List a -> List a
append Empty x       = x
append x Empty       = x
append (Cons x xs) y = Cons x (append xs y)

removeAll:: (a->Bool) -> List a -> List a
removeAll f Empty = Empty
removeAll f (Cons x xs)
                     |  f x       = (removeAll f xs)
                     | otherwise  = append (Cons x Empty) (removeAll f xs)

merge:: Ord a => List a -> List a -> List a
merge x Empty = x
merge Empty y = y
merge (Cons x xs) (Cons y ys)
                     | (x <= y)  = append (Cons x Empty) (merge xs (Cons y ys))
                     | otherwise = append (Cons y Empty) (merge (Cons x xs) ys)


mergesort:: Ord a => List a -> List a
mergesort Empty          = Empty
mergesort (Cons x Empty) = (Cons x Empty)
mergesort (Cons x xs)    = merge (mergesort (Cons x Empty)) (mergesort xs)

-- [(Cons 1 (Cons 2 (Cons 3 Empty))),(Cons 3 (Cons 5 Empty))]




