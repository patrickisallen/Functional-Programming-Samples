;; Environment 2 is implemented using 2 lists within a list. The first list stores all the variables  inside of it and the 
;; second list stores all the values of the environment. The variables and values are indexed so that each variable is associated
;;  with a value in the other list. It is simplier for finding if a variable exist but slightly less efficient in retrieving the
;; value for that variable. Extending the environment in this case is also more complex because of how it is required to build the 
;; list. It has an additional base case for when the environment is empty and another for when the environment is not empty. 
;; It builds the two lists using BOTH the list and cons functions

(define make-empty-env
    (lambda()
        '()
    )
)

(define var-index
    (lambda (var env)
        (cond
            ((null? env) (error "Variable not found"))
            ((equal? (car env) var) 
                    0)
            (else
                (+ 1 (var-index var (cdr env))))
        )
    )
)

(define val-retrieve
    (lambda (env index)
        (list-ref env index)
    )
)

(define apply-env 
    (lambda (env var)
        (cond
            ((null? env) (error "apply-env: empty environment"))
            (else 
                (val-retrieve (car (cdr env)) (var-index var (car test-env))))
        )
    )
)

(define create-var-list
    (lambda (var lst)
        (cons var lst)
    )
)

(define extend-env
    (lambda (v val env)
        (cond
            ((not (symbol? v))
                (error "invalid: not a symbol"))
            ((null? env) 
                (list (create-var-list v env) (create-var-list val env)))
            (else 
                (list (cons v (car env)) (cons val (car (cdr env)))))
        )
    )
)

(define test-env
    (extend-env 'a 1
        (extend-env 'b 2
            (extend-env 'c 3
                (extend-env 'b 4
                    (make-empty-env)
                )
            )
        )
    )
)