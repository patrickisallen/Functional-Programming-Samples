;;helper functions to check type being passed in
(define is-variable?
    (lambda (n)
        (symbol? n)
    )
)

(define is-number?
    (lambda (n)
        (number? n)
    )
)

(define is-add?
    (lambda (n)
        (equal? n '+)
    )
)

(define is-sub?
    (lambda (n)
        (equal? n '-)
    )
)

(define is-mult?
    (lambda (n)
        (equal? n '*)
    )
)

(define is-div?
    (lambda (n)
        (equal? n '/)
    )
)

(define is-expt?
    (lambda (n)
        (equal? n '**)
    )
)

(define is-zero?
    (lambda (n)
        (equal? n 0)
    )
)

(define is-one?
    (lambda (n)
        (equal? n 1)
    )
)

(define is-same-var?
    (lambda (x y)
        (and (equal? x y) (symbol? x) (symbol? y))
    )
)

(define is-dec?
    (lambda (n)
        (equal? 'dec n)
    )
)

(define is-inc?
    (lambda (n)
        (equal? 'inc n)
    )
)

(define is-not-zero-or-one?
    (lambda (n)
        (and (not (equal? n 0)) (number? n) (not (equal? n 1)))
    )
)

;;THIS IS THE REAL ONE - MADE UPDATES FROM MY OLD LISTED BELOW
(define simplify
    (lambda (expr)
        (simplifierV2 (simplifierV2 expr))
    )
)
;;simplify helper function
(define simplifierV2
    (lambda (expr)
        (cond
            ((number? expr)
                expr)
            ((is-variable? expr)
                expr)
            ((is-dec? (first expr)) ;;remove
                (list (simplifierV2 (second expr)) '- 1))
            ((is-inc? (first expr)) ;;remove
                (list (simplifierV2 (second expr)) '+ 1))
            ((list? expr)
                (let* ((op1 (simplifierV2 (first expr)))
                      (op2 (second expr))
                      (op3 (simplifierV2 (third expr))))
                      (cond
                        ((is-add? op2)
                            (cond
                                ((is-zero? op1)
                                    op3)
                                ((is-zero? op3)
                                    op1)
                                (else 
                                    (list op1 op2 op3))
                            )
                        )
                        ((is-sub? op2)
                            (cond
                                ((is-zero? op3)
                                    op1)
                                (is-same-var? op1 op3
                                    0)
                                (else 
                                    (list op1 op2 op3))
                            )
                        )
                        ((is-mult? op2)
                            (cond
                                ((is-one? op1)
                                    op3)
                                ((is-one? op3)
                                    op1)
                                ((is-zero? op1)
                                    0)
                                ((is-zero? op3)
                                    0)
                                (else 
                                    (list op1 op2 op3))
                            )
                        )
                        ((is-div? op2)
                            (cond
                                ((is-one? op3)
                                    op1)
                                (else 
                                    (list op1 op2 op3))
                            )
                        )
                        ((is-expt? op2)
                            (cond
                                ((is-zero? op3)
                                    1)
                                ((is-one? op1)
                                    1)
                                ((is-one? op3)
                                    op1)
                                (else 
                                    (list op1 op2 op3))
                            )
                        )
                      )
                ) 
            )
            (else (error "not a valid expression"))
        )
    )
)

;;initial simplifier had recursion problem, would not recurse on the highest level but the implementation above fixes that issue
(define simplifyV1
    (lambda (expr)
        (simplifier
            (simplifier 
                (simplifier 
                    (simplifier 
                        (simplifier 
                            (simplifier 
                                (simplifier expr)
                            )
                        )
                    )
                )
            )
        )
    )
)

(define simplifier
    (lambda (expr)
        (cond
            ((number? expr) 
                expr)
            ((is-variable? expr) 
                expr)
            ((is-dec? (first expr)) 
                (list (simplify (second expr)) '- 1))
            ((is-inc? (first expr)) 
                (list (simplify (second expr)) '+ 1))
            ((is-add? (second expr))
                (cond
                    ((is-zero? (first expr))
                        (simplifier (third expr)))
                    ((is-zero? (third expr))
                        (simplifier (first expr)))
                    (else 
                        (list (simplifier (first expr)) (second expr) (simplifier (third expr))))
                )
            )
            ((is-sub? (second expr))
                (cond
                    ((is-zero? (third expr))
                        (simplifier (first expr)))
                    (is-same-var? (first expr) (third expr)
                        0)
                    (else 
                        (list (simplifier (first expr)) (second expr) (simplifier (third expr))))
                )
            )
            ((is-mult? (second expr))
                (cond
                    ((is-one? (first expr))
                        (simplifier (third expr)))
                    ((is-one? (third expr))
                        (simplifier (first expr)))
                    ((is-zero? (first expr))
                        0)
                    ((is-zero? (third expr))
                        0)
                    (else 
                        (list (simplifier (first expr)) (second expr) (simplifier (third expr))))
                )
            )
            ((is-div? (second expr))
                (cond
                    (is-one? (third expr)
                        (simplifier (first expr)))
                    (else 
                        (list (simplifier (first expr)) (second expr) (simplifier (third expr))))
                )
            )
            ((is-expt? (second expr))
                (cond
                    (is-zero? (third expr)
                        1)
                    (is-one? (first expr)
                        1)
                    (is-one? (third expr)
                        (simplifier (first expr)))
                    (else 
                        (list (simplifier (first expr)) (second expr) (simplifier (third expr))))
                )
            )
            (else 
                (error "Invalid Expression"))
        )
    )
)