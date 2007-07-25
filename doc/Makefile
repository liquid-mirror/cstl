# RD converter program setting
RD2 = rd2
RD2OPT = -r rd/rd2html-lib --with-css=default.css
RD2HTML = $(RD2) $(RD2OPT) $(RD2HTMLOPT)
RMI2HTML = rmi2html.rb
RT2 = rt2
RT2HTML = $(RT2) -r rt/rt2html-lib.rb $(RTOPT) $(RT2HTMLOPT)
HTML2TXT = w3m -e -dump -T text/html
TXT2RD = txt2rd $(TXT2RDOPT)

# Files
__AllRDFiles__ = $(wildcard *.rd)
RDFiles = $(__AllRDFiles__:%.v.rd=)
RTFiles = $(wildcard *.rt)
GenerateFiles = $(HtmlByRD) $(TxtByRD) $(VerbRD) $(HtmlByRT) $(TxtByRT)
HtmlByRD = $(RDFiles:.rd=.html)
TxtByRD = $(RDFiles:.rd=.txt)
HtmlByRT = $(RTFiles:.rt=.html)
TxtByRT = $(RTFiles:.rt=.txt)

# Rules
.PHONY: rd2html rd2txt txt2rd rt2html rt2txt clean
rd2html: $(HtmlByRD)
rd2txt: $(TxtByRD)
txt2rd: $(VerbRD)
rt2html: $(HtmlByRT)
rt2txt: $(TxtByRT)
clean:
	rm -f *~ $(GenerateFiles)
%.html: %.rd
	$(RD2HTML) $< | sed -e "s/\.rd//" > $@ || rm -f $@
%.html: %.rt
	$(RD2HTML) $< | sed -e "s/\.rd//" > $@ || rm -f $@
%.txt: %.html
	$(HTML2TXT) $< > $@
%.v.rd: %
	$(TXT2RD) $< > $@
%.index.html: %.rd
	$(RD2) -r rd/rd2rmi-lib -o $* $<
	$(RMI2HTML) $*.rmi > $@

