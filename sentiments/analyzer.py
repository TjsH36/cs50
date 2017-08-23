import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives=[]
        self.negatives=[]
        with open(positives) as myfile:
            for rel in myfile:
                if rel.startswith(';')==False:
                    self.positives.append(rel.strip())
        with open(negatives) as myfile:
            for rel in myfile:
                if rel.startswith(';')==False:
                    self.negatives.append(rel.strip())

        # TODO

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        self.pos=0
        self.neg=0
        self.mid=0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        for tok in tokens:
            if tok.lower() in self.positives:
                self.pos=self.pos+1
            elif tok.lower() in self.negatives:
                self.neg=self.neg+1
            else:
                self.mid=self.mid+1
        return self.pos-self.neg
        

        # TODO
        return 0
