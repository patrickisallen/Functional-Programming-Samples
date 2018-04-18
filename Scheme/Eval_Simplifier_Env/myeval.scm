

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
;;IMPLEMENT ERROR WHEN DIVIDE BY ZERO
;;Decrement by 1
;;add 1
;;exponent

(define myeval
    (lambda (expr env)
        (cond
            ((number? expr) expr)
            ((is-variable? expr) (apply-env env expr))
            ((is-add? (second expr)) 
                (+ (myeval (first expr) env) (myeval (third expr) env)))
            ((is-sub? (second expr)) 
                (- (myeval (first expr) env) (myeval (third expr) env)))
            ((is-mult? (second expr)) 
                (* (myeval (first expr) env) (myeval (third expr) env)))
            ((is-div? (second expr)) 
                (/ (myeval (first expr) env) (myeval (third expr) env)))
            ((is-expt? (second expr)) 
                (expt (myeval (first expr) env) (myeval (third expr) env)))
            ((is-dec? (first expr))
                (- (second expr) 1))
            ((is-inc? (first expr))
                (+ (second expr) 1))
            (else (error "Not a valid operator"))
        )
    )
)


;;(define myeval-complex
;;    (lambda (expr env)
;;        (cond
;;            ((number? expr) expr)
;;            ((is-variable? expr) (apply-env env expr))
;;            ((is-add? (second expr)) 
;;                (+ (myeval-complex (first expr) env) (myeval-complex (third expr) env)))
;;            ((is-sub? (second expr)) 
;;                (- (myeval-complex (first expr) env) (myeval-complex (third expr) env)))
;;            ((is-mult? (second expr)) 
;;                (* (myeval-complex (first expr) env) (myeval-complex (third expr) env)))
;;            ((is-div? (second expr)) 
;;                (/ (myeval-complex (first expr) env) (myeval-complex (third expr) env)))
;;            ((is-expt? (second expr)) 
;;               (expt (myeval-complex (first expr) env) (myeval-complex (third expr) env)))
;;            (else (error "Not a valid operator"))
;;        )
;;    )
;;)


