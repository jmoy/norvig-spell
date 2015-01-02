#!/usr/bin/env racket
#lang racket/base

;;; Norvig's spelling corrector
;; -----------------------------------

;; Authors: 
;; Jyotirmoy Bhattacharya (jyotirmoy@jyotirmoy.net)
;; Matthias Felleisen

(provide train correct edits1)

(require racket/port)
(require racket/list)
(require racket/cmdline)

(define ALPHABET (in-list (string->list "abcdefghijklmnopqrstuvwxyz")))

;; type HFrequency = [Hashof String Nat]
;; type LFrequency = [Listof [Cons String Nat]]

;; String[Filename] -> Void
;; looks at each word on stanrd-in and prints it together with a correction 
(define (main training-file)
  (define m (train training-file))
  (for ([l (in-lines)]
        #:when (not (string=? l ""))
        [w (in-value (string-downcase l))])
    (printf "~a, ~a\n" w (correct m w))))

;; String[Filename] -> HFrequency
;; Return a hash table representing the frequency of words in the file given as an argument
;; effect: open fname for input and leave it open 
(define (train fname)
  (freqs (words (port->string (open-input-file fname)))))

;; String -> [Listof String]
;; Extracts words from a string and convert them to lowercase
(define (words buf)
  (regexp-match* #rx"[a-z]+" (string-downcase buf)))

;; [Listof String] -> HFrequency
;; Take a list of words, return a hash table with words as keys and frequencies as values
(define (freqs xs)
  (define m (make-hash))
  (for ([x (in-list xs)])
    (hash-update! m x add1 0))
  m)

;; HFrequency String -> String 
;; Returns the correction for a word.
;; Returns the word itself if no correction is found.
(define (correct m s)
  (define (best-known xs) (best (known m xs)))
  (or (best-known (list s))
      (best-known (edits1 s))
      (best-known (edits2 m s))
      s))

;; HFrequency [Listof String] -> LFrequency
;; Given a hash map and a list of words, returns a list of (word,frequency) pairs for each word
;; that is in the hash map.
(define (known m xs)
  (for*/list ([x (in-list xs)] [v (in-value (hash-ref m x #f))] #:when v)
    (cons x v)))

;; LFrequency -> String or False
;; Given a list of (word,frequency) pairs, returns one of the words with the highest frequency.
;; Given an empty list returns #f
(module+ test
  (require rackunit)
  (check-equal? (best '(("a" . 2) ("b" . 3) ("c" . 0))) "b")
  (check-true (cons? (member (best '(("a" . 2) ("b" . 3) ("c" . 3))) '("c" "b"))))
  (check-false (best '())))

(define (best xs)
  (if (empty? xs) #f (car (argmax cdr xs))))

;; String -> [Listof Sting]
;; create all words of edit distance 1
(define (edits1 s)
  (define ss (splits s))
  (append (deletes ss) (inserts ss) (replaces ss) (transposes ss)))

;; HFrequency String -> [Listof Sting]
;; create all words of edit distance 1
(define (edits2 m s)
  (for*/list ([x (in-list (edits1 s))][y (in-list (edits1 x))]#:when (hash-ref m y #f))
    y))

;; type Splits = [Listof [Cons String String]]

;; String -> Splits
;; The different ways to split a string
(define (splits s)
  (for/list ([n (in-range (add1 (string-length s)))])
    (cons (substring s 0 n) (substring s n))))

;; Splits -> [Listof String]
;; One character editing functions. Take a a split from splits and return a list of words
(module+ test
  (check-equal? (deletes '(("" . "abc") ("a" . "bc") ("ab" . "c") ("abc" . ""))) '("bc" "ac" "ab")))

(define (deletes ss)
  (for*/list ([s (in-list ss)] [rht (in-value (cdr s))] #:when (not (string=? rht "")))
    (string-append (car s) (substring rht 1))))

;; Splits -> [Listof String]
(define (inserts ss)
  (for*/list ([s (in-list ss)] [c ALPHABET])
    (string-append (car s) (string c) (cdr s))))

;; Splits -> [Listof String]
(define (replaces ss)
  (for*/list ([s (in-list ss)] [rht (in-value (cdr s))] #:when (not (string=? rht "")) [c ALPHABET])
    (string-append (car s) (string c) (substring rht 1))))

;; Splits -> [Listof String]
(define (transposes ss)
  (for*/list ([s (in-list ss)] [r (in-value (cdr s))] #:when (>= (string-length r) 2))
    (string-append (car s) (string (string-ref r 1)) (string (string-ref r 0)) (substring r 2))))

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
  
 (main training-file))
