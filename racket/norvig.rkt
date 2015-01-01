#!/usr/bin/env racket
#lang racket/base

(require racket/port)
(require racket/list)
(require racket/cmdline)

;;; Norvig's spelling corrector

;; (c) Jyotirmoy Bhattacharya
;; jyotirmoy@jyotirmoy.net

(provide train correct edits1)

;; Take a list of words, return a hash table
;; with words as keys and frequencies as values
(define (freqs xs)
  (define m (make-hash))
  (for ([x (in-list xs)])
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
(define alphabet 
  (map string (string->list "abcdefghijklmnopqrstuvwxyz")))

;; The different ways to split a string
(define (splits s)
  (for/list
      ([n (in-range (add1 (string-length s)))])
    (cons (substring s 0 n) (substring s n))))

;; One character editing functions. Take a a split
;; from splits and return a list of words
(define (deletes ss)
  (for*/list
      ([s (in-list ss)]
       [lft (in-value (car s))]
       [rht (in-value (cdr s))]
       #:when (not (string=? rht "")))
    (string-append lft (substring rht 1))))

(define (inserts ss)
  (for*/list
      ([s (in-list ss)]
       [lft (in-value (car s))]
       [rht (in-value (cdr s))]
       [c (in-list alphabet)])
    (string-append lft c rht)))

(define (replaces ss)
  (for*/list
      ([s (in-list ss)]
       [lft (in-value (car s))]
       [rht (in-value (cdr s))]
       #:when (not (string=? rht ""))
       [c (in-list alphabet)])
    (string-append lft c (substring rht 1))))

(define (transposes ss)
  (for*/list
      ([s (in-list ss)]
       [lft (in-value (car s))]
       [rht (in-value (cdr s))]
       #:when (>= (string-length rht) 2))
    (string-append 
      lft 
      (string (string-ref rht 1))
      (string (string-ref rht 0))
      (substring rht 2))))

(define (edits1 s)
  (define ss (splits s))
  (append 
   (deletes ss)
   (inserts ss)
   (replaces ss)
   (transposes ss)))

;; Given a hash map and a list of words, returns
;; a list of (word,frequency) pairs for each word
;; that is in the hash map.
(define (known m xs)
  (for*/list
      ([x (in-list xs)]
       [v (in-value (hash-ref m x #f))]
       #:when v)
    (cons x v)))

;; Given a list of (word,frequency) pairs, returns 
;; one of the words with the highest frequency.
;; Given an empty list returns #f
(define (best xs)
  (define best-pair
    (for/fold ([bst (cons #f 0)])
      ([x (in-list xs)])
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
  
  (for ([l (in-lines)]
        #:when (not (empty? l))
        [w (in-value (string-downcase l))])
    (printf "~a, ~a\n" w (correct m w))))


