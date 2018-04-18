;;This environment is constructed using "key-value pairs". 
;; The environment stores variables and values together as a pair inside a list
;; So a list is stored like so: ((a 3) (b 2) (c 4))
;; It is fairly simple to retrieve a variable by calling the caar on the pair to find if a variable exist and cdar on the
;; pair to find the value of the variable
;; Extending the environment is also simple because it just appends a pair to a list using cons and cons to create the initial pair
;; that needs to be extended

(define make-empty-env
    (lambda()
        '()
    )
)

(define apply-env 
    (lambda (env var)
        (cond
            ((null? env) (error "apply-env: empty environment"))
            ((equal? (caar env) var) (cdar env))
            (else (apply-env (cdr env) var))
        )
    )
)

(define extend-env
    (lambda (v val env)
        (cond
            ((not (symbol? v))
                (error "invalid: not a symbol"))
            (else
                (cons (cons v val) env))
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