(define singleton?
	(lambda(array)
		(cond
			((not(list? array)) 
				#f)
			((not(null?(cdr array))) 
				#f)
			(else 
				#t)
		)
	)
)

(define my-make-list
	(lambda (val1 arr)
		(cond
			((equal? val1 1) 
				(list arr))
			((null? arr) 
				'())
			(else 
				(cons arr (my-make-list(- val1 1) arr)))
		)
	)
)

(define my-iota-helper
	(lambda (x y)
		(cond
			((equal? x 1) 
				(list (- y x)))
			(else 
				(cons (- y x) (my-iota-helper (- x 1) y)))
		)
	)
)

(define my-iota
	(lambda (x)
		(cond
			((equal? x 0) 
				0)
			(else 
				(my-iota-helper x x))
		)
	)
)

;;

(define len
	(lambda (lst)
		(cond
			((null? lst) 
				0)
			((not(list? lst)) 
				#f)
			(else 
				(+ 1 (len (cdr lst))))
		)
	)
)

(define nth
	(lambda (lst iter)
		(cond
			((not(list? lst)) 
				'(not a list))
			((null? lst) 
				'())
			((>= iter (len lst)) 
				'(out of bounds))
			((equal? iter 0) 
				(car lst))
			(else 
				(nth (cdr lst) (- iter 1)))
		)
	)
)

(define my-last
	(lambda (lst)
		(cond
			((not(list? lst)) 
				'(not a list))
			((null? lst) 
				'())
			((singleton? lst) 
				(car lst))
			(else 
				(my-last (cdr lst)))
		)
	)
)


(define my-filter
	(lambda (pred lst)
		(cond
			((null? lst) 
				'())
			((pred (car lst))
			 	(cons (car lst) (my-filter pred (cdr lst))))
			(else 
				(my-filter pred (cdr lst)))
		)
	)
)

(define my-append
	(lambda (a b)
		(cond
			((null? a) 
				b)
			(else 
				(cons (car a) (my-append (cdr a) b)))
		)
	)
)

;;scheme notes 
(define append-all
	(lambda (lst)
		(cond
			((null? lst) 
				'())
			((not (list? lst)
				lst))
			((list? (car lst)) 
				(my-append (append-all (car lst)) 
						   (append-all (cdr lst))))
			(else 
				(cons (car lst) 
				(append-all (cdr lst))))
		)
	)
)

;;---------------------------Mergesort Implementation ---------------------
(define every-other-even
  	(lambda (lst)
  		(cond
    		((null? lst) 
    			'())
    		(else 
    			(cons (car lst) (every-other-odd (cdr lst))))
    	)
    )
 )

(define every-other-odd
	(lambda (lst)
  		(cond
    		((null? lst) 
    			'())
    		(else 
    			(every-other-even (cdr lst)))
    	)
  	)
)

(define split
	(lambda (lst)
		(cond
			((null? lst) 
				'())
			((null? (cdr lst)) '())
			(else 
				(list (every-other-even lst)
					  (every-other-odd lst)))	
		)
	)
)

(define merge
	(lambda (first-half second-half)
		(cond
			((null? first-half) 
				second-half)
			((null? second-half) 
				first-half)
			((< (car first-half) (car second-half))
				(cons (car first-half) (merge (cdr first-half) second-half)))
			(else
				(cons (car second-half) (merge (cdr second-half) first-half)))
		)
	)
)

(define my-sort
	(lambda (lst)
		(cond
			((null? lst) lst)
			((= (len lst) 1) lst)
			(else 
				(merge (my-sort (car (split lst))) 
					    (my-sort (cadr (split lst))))
			) 
		)
	)
)
;;--------------------------------------------------------------------------------

;;Citation: https://www.youtube.com/watch?v=UKiklIGW1Cs
(define binary-convert
	(lambda (n)
		(cond
			((equal? n 0) 0)
			(else (+ (* 10 (binary-convert (quotient n 2))) (remainder n 2)))
		)
	)
)

;;https://stackoverflow.com/questions/23549344/drracket-and-recursive-statement-binary-to-decimal
(define decimal-convert
	(lambda (n)
		(cond
			((zero? n) 
				n)
			(else 
				(+ (modulo n 10) (* 2 (decimal-convert (quotient n 10)))))
		)
	)
)


(define my-map
	(lambda (fn lst)
		(cond
			((null? lst) '())
			(else
				(cons (fn (car lst))
					  (my-map fn (cdr lst))))
		)
	)
)

(define compute-max-num
	(lambda (n)
		(expt 2 n)
	)
)

(define create-decimal-list
	(lambda (n)
		(my-iota (compute-max-num n))
	)
)

(define convert-list-binary
	(lambda (lst)
		(my-map (lambda (n) (binary-convert n)) lst)
	)
)

(define bracket-list
	(lambda (lst)
		(cond
			((null? lst) 
				'())
			(else 
				(cons (list (car lst)) (bracket-list (cdr lst))))
		)
	)
)

(define all-bits
	(lambda (n)
		(cond
			((equal? 0 n) 
				'())
			(else 
				(bracket-list (convert-list-binary (create-decimal-list n))))
		)
	)
)

