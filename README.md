
<p align="center">
  <!--<img src="https://raw.githubusercontent.com/GregHilston/Global-Game-Jam-2019/master/Global-Game-Jam-2019/Assets/Art/Home-is-Where.png">-->
</p>

<p align="center">
  <!--<img src="https://raw.githubusercontent.com/GregHilston/Global-Game-Jam-2019/master/Global-Game-Jam-2019/Assets/Art/in_game.png">-->
</p>

> "Break into homes and establish it as your home"

This project was completed in a 48 hour session called [Global Game Jam](https://globalgamejam.org/). The 2019 theme was 

> What does home mean to you?

Our group decided to take that concept and design a game where the main player breaks into someone else's home and races to replace their belonging with their own.

Our main technical focus was on random generation given a input seed phrase. This design was so that players can compete by trying to acheive the lowest time to complete the generated map. Utilizing ~1,000 lines of custom C#, we take in a input seed and generate an MD5 hash. This hash is used to pull out values that we set for generation of the map. 

We specifically seed the randomizes used, so we can non-stochastically generate our maps. Essentially, given the same input string, we should always generate the same map. This allows users to compete on the same map by exchanging input seed strings.



- [Tyler Schmidt: Software Engineer](https://github.com/downhillGames)
