theory rule_type
  imports Main
begin

datatype 'a tree = Tip | Node "'a tree " 'a "'a tree " 


fun two_fib::" nat => nat" where
"two_fib 0 = 1" |
"two_fib (Suc 0) = 1" |
"two_fib n = (two_fib (n-1)) + (two_fib (n-2))"

fun three_fib::" nat => nat" where
"three_fib 0 = 1" |
"three_fib (Suc 0) = 1" |
"three_fib (Suc (Suc 0)) = 1" |
"three_fib n = (three_fib (n-1) + three_fib (n-2) + three_fib(n-3))"

fun four_fib::" nat => nat" where
"four_fib 0 = 1" |
"four_fib (Suc 0) = 1" |
"four_fib (Suc (Suc 0)) = 1" |
"four_fib (Suc (Suc (Suc 0))) = 1" |
"four_fib n = (four_fib(n-1) + four_fib(n-2) + four_fib(n-3) + four_fib(n-4))"

fun Jacobsthal::" nat => nat" where
"Jacobsthal 0 = 2" |
"Jacobsthal (Suc 0) = 1" |
"Jacobsthal n = (Jacobsthal (n-1)) + 2*(Jacobsthal (n-2))"

fun Narayana::" nat => nat" where
"Narayana 0 = 1" |
"Narayana (Suc 0) = 1" |
"Narayana (Suc (Suc 0)) = 1" |
"Narayana n = Narayana(n-1) + Narayana(n-3)"

fun Leonardo::" nat => nat" where
"Leonardo 0 = 1" |
"Leonardo (Suc 0) = 1" |
"Leonardo n = Leonardo(n-1) + Leonardo(n-2) + 1"

fun Padua::" nat => nat" where
"Padua 0 = 1" |
"Padua (Suc 0) = 1" |
"Padua (Suc (Suc 0)) = 1" |
"Padua n = Padua(n-2) + Padua(n-3)"

fun Pell::" nat => nat" where
"Pell 0 = 0" |
"Pell (Suc 0) = 1" |
"Pell n = (Pell(n-1)*2)+Pell(n-2)"

fun Lucas::" nat => nat" where
"Lucas 0 = 2" |
"Lucas (Suc 0) = 1" |
"Lucas n = Lucas(n-1) + Lucas(n-2)"


end