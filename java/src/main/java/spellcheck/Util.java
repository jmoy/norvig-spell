package spellcheck;

import java.util.Collection;
import java.util.Spliterators;
import java.util.function.Consumer;
import java.util.function.Predicate;
import java.util.function.Supplier;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;
import java.util.stream.StreamSupport;

import static java.util.Arrays.copyOfRange;

// Should be in standard library
public class Util {
  @SafeVarargs
  static Stream<String> concat(Stream<String>... streams) {
    return Stream.of(streams).flatMap(s -> s);
  }

  @SafeVarargs
  static <T extends Collection<V>, V> T or(T f, Supplier<T>... s) {
    return f.isEmpty() ? s.length == 1 ? s[0].get() : or(s[0].get(), copyOfRange(s, 1, s.length)) : f;
  }

  static Stream<String> find(String regex, String string) {
    Matcher matcher = Pattern.compile(regex).matcher(string);
    return stream(action -> {
      boolean b = matcher.find();
      if (b) {
        action.accept(matcher.group());
      }
      return b;
    });
  }

  private static class PredicateSpliterator<T> extends Spliterators.AbstractSpliterator<T> {

    private final Predicate<Consumer<? super T>> predicate;

    PredicateSpliterator(Predicate<Consumer<? super T>> predicate) {
      super(1, 0);
      this.predicate = predicate;
    }

    @Override
    public boolean tryAdvance(Consumer<? super T> action) {
      return predicate.test(action);
    }
  }

  static <T> Stream<T> stream(Predicate<Consumer<? super T>> predicate) {
    return StreamSupport.stream(new PredicateSpliterator<>(predicate), false);
  }
}
