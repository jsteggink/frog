/*
  $Id$
  $URL$

  Copyright (c) 2006 - 2015
  Tilburg University

  This file is part of frog.

  frog is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  frog is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  For questions and suggestions, see:
      http://ilk.uvt.nl/software.html
  or send mail to:
      timbl@uvt.nl
*/

#ifndef MBMA_MOD_H
#define MBMA_MOD_H

#include <unicode/translit.h>

class MBMAana;
class RulePart;

namespace CLEX {
  enum Type { UNASS, N, A, Q, V, D, O, B, P, Y, I, X, Z, PN, AFFIX, XAFFIX,
	      NEUTRAL };
  std::string toString( const Type& t );
}

enum Status { INFO, STEM, COMPLEX, INFLECTION, DERIVATIONAL };

class BaseBracket {
public:
 BaseBracket( CLEX::Type t, const std::vector<CLEX::Type>& R, int flag ):
  RightHand(R),
    cls(t),
    debugFlag(flag)

   {};
 BaseBracket( CLEX::Type t, int flag ):
  cls(t), debugFlag(flag)
  {};
  virtual ~BaseBracket() {};

  Status stat() const { return status; };
  virtual UnicodeString morpheme() const { return "";};
  virtual UnicodeString deepmorphemes() const { return "";};
  virtual std::string inflection() const { return ""; };
  virtual std::string original() const { return ""; };
  virtual int infixpos() const { return -1; };
  virtual UnicodeString put( bool = false ) const;
  virtual BaseBracket *append( BaseBracket * ){ abort(); };
  virtual bool isNested() { return false; };
  virtual void resolveLead(){ abort(); };
  virtual void resolveTail(){ abort(); };
  virtual void resolveMiddle(){ abort(); };
  virtual folia::Morpheme *createMorpheme( folia::Document *,
					   const std::string&,
					   const std::string& ) const = 0;
  virtual folia::Morpheme *createMorpheme( folia::Document *,
					   const std::string&,
					   const std::string&,
					   int&,
					   std::string& ) const = 0;
  CLEX::Type tag() const { return cls; };
  void setTag( CLEX::Type t ) { cls = t; };
  std::vector<CLEX::Type> RightHand;
 protected:
  CLEX::Type cls;
  Status status;
  int debugFlag;
};

class BracketLeaf: public BaseBracket {
public:
  BracketLeaf( const RulePart&, int );
  BracketLeaf( CLEX::Type, const UnicodeString&, int );
  UnicodeString put( bool = false ) const;
  UnicodeString morpheme() const { return morph; };
  UnicodeString deepmorphemes() const { return morph; };
  std::string inflection() const { return inflect; };
  std::string original() const { return orig; };
  int infixpos() const { return ifpos; };
  folia::Morpheme *createMorpheme( folia::Document *,
				   const std::string&,
				   const std::string& ) const;
  folia::Morpheme *createMorpheme( folia::Document *,
				   const std::string&,
				   const std::string&,
				   int&,
				   std::string& ) const;
private:
  int ifpos;
  UnicodeString morph;
  std::string orig;
  std::string inflect;
};

class BracketNest: public BaseBracket {
 public:
 BracketNest( CLEX::Type t, int flag ): BaseBracket( t, flag ){
    status = COMPLEX; };
  BaseBracket *append( BaseBracket *t ){
    parts.push_back( t );
    return this;
  };
  ~BracketNest();
  bool isNested() { return true; };
  UnicodeString put( bool = false ) const;
  bool testMatch( std::list<BaseBracket*>& result,
		  const std::list<BaseBracket*>::iterator& rpos,
		  std::list<BaseBracket*>::iterator& bpos );
  std::list<BaseBracket*>::iterator resolveAffix( std::list<BaseBracket*>&,
						  const std::list<BaseBracket*>::iterator& );
  void resolveNouns();
  void resolveLead();
  void resolveTail();
  void resolveMiddle();
  UnicodeString deepmorphemes() const;
  CLEX::Type getFinalTag();
  folia::Morpheme *createMorpheme( folia::Document *,
				   const std::string&,
				   const std::string& ) const;
  folia::Morpheme *createMorpheme( folia::Document *,
				   const std::string&,
				   const std::string&,
				   int&,
				   std::string& ) const;
  std::list<BaseBracket *> parts;
};


class RulePart {
public:
  RulePart( const std::string&, const UChar );
  bool isBasic() const;
  void get_ins_del( const std::string& );
  CLEX::Type ResultClass;
  std::vector<CLEX::Type> RightHand;
  UnicodeString ins;
  UnicodeString del;
  UnicodeString uchar;
  UnicodeString morpheme;
  std::string inflect;
  int fixpos;
  int xfixpos;
  bool participle;
};


class Rule {
public:
  Rule( const std::vector<std::string>&, const UnicodeString&, int flag );
  std::vector<std::string> extract_morphemes() const;
  std::string getCleanInflect() const;
  void reduceZeroNodes();
  BracketNest *resolveBrackets( bool, CLEX::Type& );
  std::vector<RulePart> rules;
  int debugFlag;
};

static std::map<CLEX::Type,std::string> tagNames;
static std::map<char,std::string> iNames;

class Mbma {
 public:
  Mbma(TiCC::LogStream *);
  ~Mbma();
  bool init( const TiCC::Configuration& );
  void addDeclaration( folia::Document& doc ) const;
  void Classify( folia::Word * );
  void Classify( const UnicodeString& );
  void filterTag( const std::string&, const std::vector<std::string>& );
  std::vector<std::vector<std::string> > getResult() const;
  void setDaring( bool b ){ doDaring = b; };
  void clearAnalysis();
  std::string getTagset() const { return mbma_tagset; };
 private:
  void cleanUp();
  bool readsettings( const std::string&, const std::string& );
  void fillMaps();
  void init_cgn( const std::string& );
  Transliterator * init_trans();
  UnicodeString filterDiacritics( const UnicodeString& ) const;
  void getFoLiAResult( folia::Word *, const UnicodeString& ) const;
  std::vector<std::string> make_instances( const UnicodeString& word );
  bool performEdits( Rule& );
  void resolve_inflections( Rule& );
  CLEX::Type getFinalTag( const std::list<BaseBracket*>& );
  void execute( const UnicodeString& word,
		const std::vector<std::string>& classes );
  int debugFlag;
  void addMorph( folia::MorphologyLayer *,
		 const std::vector<std::string>& ) const;
  void addMorph( folia::Word *, const std::vector<std::string>& ) const;
  void addBracketMorph( folia::Word *,
			const std::string&,
			const std::string& ) const;
  void addBracketMorph( folia::Word *, const BracketNest * ) const;
  std::string MTreeFilename;
  Timbl::TimblAPI *MTree;
  std::map<std::string,std::string> TAGconv;
  std::vector<MBMAana*> analysis;
  std::string version;
  std::string mbma_tagset;
  std::string cgn_tagset;
  std::string clex_tagset;
  TiCC::LogStream *mbmaLog;
  Transliterator *transliterator;
  Tokenizer::UnicodeFilter *filter;
  bool doDaring;
};

class MBMAana {
  friend std::ostream& operator<< ( std::ostream& , const MBMAana& );
  friend std::ostream& operator<< ( std::ostream& , const MBMAana* );
  public:
  MBMAana(const Rule&, bool );

  ~MBMAana() { delete brackets; };

  std::string getTag() const {
    return tag;
  };

  const Rule& getRule() const {
    return rule;
  };

  const BracketNest *getBrackets() const {
    return brackets;
  };

  std::string getInflection() const {
    return infl;
  };

  const std::vector<std::string> getMorph() const {
    return rule.extract_morphemes();
  };

  UnicodeString getKey( bool );

 private:
  std::string tag;
  std::string infl;
  UnicodeString sortkey;
  std::string description;
  Rule rule;
  BracketNest *brackets;
};

#endif
