{-# LANGUAGE OverloadedStrings,TupleSections #-}
import System.Environment
import Data.Char
import Data.List (foldl')
import Control.Monad
import Control.Applicative
import Data.Maybe
import qualified Data.ByteString.Char8 as S
import qualified Data.Trie as T
import qualified Data.Trie.Convenience as TC

type Token = S.ByteString
type Model = T.Trie Int

alphabet::[Token]
alphabet = map S.singleton ['a'..'z']

tokenise::Token->[Token]
tokenise s = [S.map toLower w|
              w<-S.splitWith (not.isAlpha) s,
              (not . S.null) w]

train::[Token]->Model
train = foldl' inserter T.empty 
  where
    inserter m k = TC.insertWith (+) k 1 m

edit1::Token->[Token]
edit1 s = concat [deletes,replaces,transposes,inserts]
  where
    splits      = [S.splitAt i s|i<-[0..S.length s]]
    
    inserts     = [S.concat [a,c,b]
                  |(a,b)<-splits, c<-alphabet]
                  
    replaces    = [S.concat [a,c,S.tail b]
                  |(a,b)<-splits, S.length b>0,c<-alphabet]
                  
    deletes     = [a `S.append` S.tail b
                  | (a,b) <- splits, S.length b > 0]
                  
    transposes  = [S.concat [a,r,l,rest]
                  | (a,b) <- splits,
                    S.length b > 1,
                    let l = S.take 1 b,
                    let r = S.take 1 (S.tail b),
                    let rest = S.drop 2 b]


-- Find correction according to a lexicographic ordering
-- which first minimizes edit distance and then maximizes
-- frequency in the training set. Return input if no correction
-- is found.
correct::Model->Token->Token
correct model s= maybe s (snd.maximum) candidates
  where
    -- Words at edit distance 1 from s
    e1 = edit1 s
    -- Words at edit distance 2 from s
    e2 = concatMap edit1 e1
    -- Mark with frequency if known, otherwise Nothing
    knownMark w= (,w) <$> (w `T.lookup` model)
    -- Marked known words from the shortest edit distance
    -- upto 2 where there are any known words. Otherwise Nothing.
    candidates = listToMaybe $
                 [known|
                 edits <- [[s],e1,e2],
                 let known = mapMaybe knownMark edits,
                 (not.null) known]

main::IO()
main = do
  args <- getArgs
  let trainset = 
        case args of
          ([x]) -> x
          _ -> error "Usage: norvig [training data file]"
  trw <- tokenise <$> S.readFile trainset
  let model = train trw
  inw <- (S.lines . S.map toLower) <$> S.getContents
  forM_ inw $ \w-> 
    S.putStrLn (S.concat [w,", ",correct model w])
       
