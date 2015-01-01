#!/usr/bin/env racket
#lang racket
(define (freqs xs)
  (define m (make-hash))
  (for [(x xs)]
    (hash-update! m x add1 0))
  m)

(define (words buf)
  (for/list 
      ([w (regexp-split #rx"[^a-zA-z]+" buf)]
       #:when (not (equal? w "")))
    (string-downcase w)))

(define (train fname)
  (call-with-input-file fname
    (lambda (fp) (freqs (words (port->string fp))))))

(define alphabet "abcdefghijklmnopqrstuvwxyz")

(define (deletes s)
  (for/list
      ([n (in-range (string-length s))])
    (string-append 
     (substring s 0 n)
     (substring s (+ 1 n)))))

(define (inserts s)
  (for*/list
      ([n (in-range (+ 1 (string-length s)))]
       [c alphabet])
    (string-append 
     (substring s 0 n)
     (string c)
     (substring s n))))
    
(define (replaces s)
  (for*/list
      ([n (in-range (string-length s))]
       [c alphabet])
    (string-append 
     (substring s 0 n)
     (string c)
     (substring s (+ 1 n)))))

(define (transposes s)
  (for/list
      ([n (in-range (- (string-length s) 1))])
    (string-append
     (substring s 0 n)
     (string (string-ref s (+ 1 n)))
     (string (string-ref s n))
     (substring s (+ 2 n)))))

(define (edits1 s)
  (for*/list
      ([f (list deletes inserts replaces transposes)]
       [w (f s)])
    w))

(define (known m xs)
  (for*/list
      ([x xs]
       [v (in-value (hash-ref m x 0))]
       #:when (> v 0))
    (cons x v)))
       
(define (best xs)
  (for/fold ([bst (cons "" 0)])
    ([x xs])
    (if (> (cdr x) (cdr bst)) x bst)))

(define (correct m s)
  (if (hash-has-key? m s) s (correct1 m s)))

(define (correct1 m s)
  (let* [(e1r (edits1 s))
         (e1 (known m e1r))]
    (if (not (empty? e1))
        (car (best e1))
        (correct2 m e1r s))))

(define (correct2 m e1 s)
  (let [(e2 (known m
                   (for*/list 
                       ([e e1]
                        [w (edits1 e)])
                     w)))]
    (if (not (empty? e2))
        (car (best e2))
        s)))

(define (correct-all m in)
  (for*/list
      [(l (in-lines in))
       (w (in-value (string-downcase l)))]
    (cons w (correct m w))))

(define training-file
  (command-line
   #:program "norvig"
   #:args (filename)
   filename))

(let [(m (train training-file))]
  (for ([wp (correct-all m (current-input-port))])
    (printf "~a, ~a\n" (car wp) (cdr wp))))
