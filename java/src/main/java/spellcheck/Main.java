package spellcheck;

import java.io.*;
import java.nio.file.Files;
import java.util.*;
import java.util.stream.Stream;

import static java.util.Arrays.asList;
import static java.util.stream.Collectors.*;
import static java.util.stream.IntStream.range;
import static spellcheck.Util.*;

public class Main {

  static Stream<String> edit(String w) {
    String alphabet = "abcdefghijklmnopqrstuvwxyz";
    int len = w.length();
    Stream<String> deletes = range(0, len - 1).mapToObj(i -> w.substring(0, i) + w.substring(i + 1));
    Stream<String> transposes = range(0, len - 1)
            .mapToObj(i -> w.substring(0, i) + w.substring(i + 1, i + 2) + w.substring(i, i + 1) + w.substring(i + 2));
    Stream<String> replaces = range(0, len).boxed()
            .flatMap(i -> alphabet.chars().mapToObj(c -> w.substring(0, i) + (char) c + w.substring(i + 1)));
    Stream<String> inserts = range(0, len).boxed()
            .flatMap(i -> alphabet.chars().mapToObj(c -> w.substring(0, i) + (char) c + w.substring(i)));
    return concat(deletes, transposes, replaces, inserts);
  }

  static Stream<String> edits(String word) {
    return edit(word).flatMap(Main::edit);
  }

  static Collection<String> known(Stream<String> words) {
    return words.filter(NWORDS::containsKey).distinct().sorted((s1, s2) -> NWORDS.get(s2) - NWORDS.get(s1)).collect(toList());
  }

  static Collection<String> correct(String word) {
    return or(known(Stream.of(word)), () -> known(edit(word)), () -> known(edits(word)), () -> asList(word));
  }

  static Map<String, Integer> NWORDS;

  public static void main(String[] args) throws IOException {
    NWORDS = Files.lines(new File(args[0]).toPath())
            .map(String::toLowerCase)
            .flatMap(s -> find("[a-z]+", s))
            .collect(toMap(s -> s, s -> 1, (v1, v2) -> v1 + v2));

    String line;
    while ((line = System.console().readLine()) != null) {
      System.out.println(correct(line.trim()));
    }
  }
}