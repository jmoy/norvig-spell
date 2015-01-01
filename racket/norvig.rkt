#!/usr/bin/env racket
#lang racket
(provide train correct edits1)

(define (freqs xs)
  (define m (make-hash))
  (for [(x xs)]
    (hash-update! m x add1 0))
  m)

(define (words buf)
  (regexp-match* #rx"[a-z]+" 
                 (string-downcase buf)))

(define (train fname)
  (call-with-input-file fname
    (lambda (fp) (freqs (words (port->string fp))))))

(define alphabet "abcdefghijklmnopqrstuvwxyz")

(define (deletes s)
  (for/list
      ([n (in-range (string-length s))])
    (string-append 
     (substring s 0 n)
     (substring s (add1 n)))))

(define (inserts s)
  (for*/list
      ([n (in-range (add1 (string-length s)))]
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
     (substring s (add1 n)))))

(define (transposes s)
  (for/list
      ([n (in-range (- (string-length s) 1))])
    (string-append
     (substring s 0 n)
     (string (string-ref s (add1 n)))
     (string (string-ref s n))
     (substring s (+ 2 n)))))

(define (edits1 s)
  (append 
   (deletes s)
   (inserts s)
   (replaces s)
   (transposes s)))

(define (known m xs)
  (for*/list
      ([x xs]
       [v (in-value (hash-ref m x #f))]
       #:when v)
    (cons x v)))

(define (best xs)
  (define best-pair
    (for/fold ([bst (cons #f 0)])
      ([x xs])
      (if (> (cdr x) (cdr bst)) x bst)))
  (car best-pair))

(define (correct m s)
  (define (best-known xs) (best (known m xs)))
  (or
   (best-known (list s))
   (best-known (edits1 s))
   (best-known (append-map edits1 (edits1 s)))
   s))

(define (correct-all m in)
  (for*/list
      [(l (in-lines in))
       (w (in-value (string-downcase l)))]
    (cons w (correct m w))))

(module+ main
  (define (training-file)
    (command-line
     #:program "norvig"
     #:args (filename)
     filename))
  
  (let [(m (train (training-file)))]
    (for ([wp (correct-all m (current-input-port))])
      (printf "~a, ~a\n" (car wp) (cdr wp)))))
