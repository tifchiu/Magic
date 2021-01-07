# Magic!
Magic! is a command-line card game resembling Yu-Gi-Oh!, or Magic: The Gathering. The various cards and abilities are implemented using objective-C++ strategies (polymorphism and the Observer design pattern, etc.).

## How to play
After running 
    make you can begin a game on the command-line by running the following (with optional -flags)  
```
./sorcery -testing -deck1 deck1file.txt -deck2 deck2file.txt -init commands.txt
      // in -testing mode, the decks are not shuffled, and you are able to draw and discard
      // using either of -deck1 and/or -deck2 followed by a valid text file of cards will give Player 1/2 this unique deck of cards
      // using -init followed by a text file of commands will start a game of Magic! using commands from the provided file first
```

### TO DO:  
* Add a more elegant GUI  
* Consider adding more default decks to support similar card games  
