JAVA = ChainWordCount.java
JARS = $(JAVA:.java=.jar)

JAVAC = javac -classpath "lib/*"

.PHONY: all
all: $(JARS)
	$(info All ok.)

%.jar: %.java
	$(JAVAC) -d . $^
	jar cf $@ ./*



.PHONY: clean
clean:
	[ ! -d BUILD ] || rm -rf BUILD
