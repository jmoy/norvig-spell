#!/usr/bin/env racket
#lang racket

;;; Norvig's spelling corrector

;; (c) Jyotirmoy Bhattacharya
;; jyotirmoy@jyotirmoy.net

(provide train correct edits1)

;; Take a list of words, return a hash table
;; with words as keys and frequencies as values
(define (freqs xs)
  (define m (make-hash))
  (for [(x xs)]
    (hash-update! m x add1 0))
  m)

;; Extracts words from a string and convert them
;; to lowercase
(define (words buf)
  (regexp-match* #rx"[a-z]+" 
                 (string-downcase buf)))

;; Return a hash table representing the frequency
;; of words in the file given as an argument
(define (train fname)
  (call-with-input-file fname
    (lambda (fp) (freqs (words (port->string fp))))))

;; All the allowed letters in a word
(define alphabet "abcdefghijklmnopqrstuvwxyz")

;; One character editing functions. Take a word
;; return a list of words
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

;; Given a hash map and a list of words, returns
;; a list of (word,frequency) pairs for each word
;; that is in the hash map.
(define (known m xs)
  (for*/list
      ([x xs]
       [v (in-value (hash-ref m x #f))]
       #:when v)
    (cons x v)))

;; Given a list of (word,frequency) pairs, returns 
;; one of the words with the highest frequency.
;; Given an empty list returns #f
(define (best xs)
  (define best-pair
    (for/fold ([bst (cons #f 0)])
      ([x xs])
      (if (> (cdr x) (cdr bst)) x bst)))
  (car best-pair))

;; Returns the correction for a word.
;; Returns the word itself if no correction is found.
(define (correct m s)
  (define (best-known xs) (best (known m xs)))
  (or
   (best-known (list s))
   (best-known (edits1 s))
   (best-known (append-map edits1 (edits1 s)))
   s))

;; Reads one word from line from the input port 'in'.
;; Return a list of (word,correction) pairs.
(define (correct-all m in)
  (for*/list
      [(l (in-lines in))
       (w (in-value (string-downcase l)))]
    (cons w (correct m w))))

(module+ main
;; The main program.
;; Must be called as 
;;    norvig.rkt [training file]
;; Learns word frequencies from the [training file],
;; then reads one word per line from standard
;; input and print lines of the form
;;    word, correction
;; to standard output.
  (define training-file
    (command-line
     #:program "norvig"
     #:args (filename)
     filename))
  
  (define m (train training-file))

  (for ([wp (correct-all m (current-input-port))])
    (printf "~a, ~a\n" (car wp) (cdr wp))))


