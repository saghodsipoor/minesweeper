#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Minesweeper.hpp"


void Minesweeper::start()
{
  int w = cell_width_ * ( game_->size().w + 2); // 2 for border
  int h = cell_width_ * ( game_->size().h + 2); // 2 for border

  window_.create(sf::VideoMode(w, h), "Minesweeper",
    sf::Style::Titlebar | sf::Style::Close);

  while (window_.isOpen())
  {
    sf::Event event;
    while (window_.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window_.close();
      else
      if (event.type == sf::Event::KeyPressed)
      {
        if (event.key.code == sf::Keyboard::S)
        {
          just_entered_the_game_ = false;
          just_finished_the_game_ = true;
          game_->reset();
        }
      }
      else
      if (event.type == sf::Event::MouseButtonPressed)
      {
        if (!game_->is_on())
          break;

        Game::Index index =
          {event.mouseButton.x / 16 - 1, event.mouseButton.y / 16 - 1};
        
        if (!game_->index_is_valid(index))
          break;

        if (event.mouseButton.button == sf::Mouse::Left)
          game_->visit(index);
        else
        if (event.mouseButton.button == sf::Mouse::Right)
          game_->toggle_flag(index); 
      }
    }
    display();
  }
}

void Minesweeper::display()
{
  window_.clear( {190, 190, 190} );

  struct {int x, y;} origin = {cell_width_, cell_width_};
  
  if (just_entered_the_game_)
  {
    window_.draw(welcome_text_);
    window_.display();
    return;
  }

  game_->for_each_index([&](int i, int j){
    auto state = game_->cell_state({i,j});
    sprites_[state].setPosition(i*16 + origin.x, j*16 + origin.y);
    window_.draw(sprites_[state]);
  });
  
  if (game_->state() != Game::On)
  {
    if (just_finished_the_game_)
    {
      ending_thread_ = std::thread(
        [&]() {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          just_finished_the_game_ = false;
        }
      );
      ending_thread_.detach();
      window_.display();
      return;
    }
    window_.clear( {190, 190, 190} );
    window_.draw( game_->state() == Game::Won ? win_text_ : lose_text_ );
  }

  window_.display();
}

void Minesweeper::load_resources()
{ 
  if (!sprite_sheet_.loadFromFile("sprites/sprites.png",
    sf::IntRect(2, 53, 135, 33) ))
      throw("Couldn't load sprites/sprites.png");

  if (!font_.loadFromFile("fonts/Inconsolata-Regular.ttf"))
    throw std::runtime_error("coudn't load the font Inconsolata-Regular.ttf");

  // loading digit sprites
  for (int i = 1; i < 9; ++i)
  {
    sf::Sprite sprite(sprite_sheet_, sf::IntRect(17 * (i-1), 17, 16, 16));
    sprites_.emplace(std::to_string(i), sprite);
  }

  welcome_text_.setString(
    "In the Name of God.\n"
    "Welcome to the\n" 
    "Minesweeper!\n"
    "Press S to\n"
    "start/restart\n"
    "the game.\n"
  );
  win_text_.setString(
    "     You win :)    \n"
    "Press S to restart.\n"
  );
  lose_text_.setString(
    "  You lose man :(  \n"
    "Press S to restart.\n"
  );
  
  //positioning text
  for (auto& t : std::vector<sf::Text*>{&welcome_text_, &win_text_, &lose_text_})
  {
    t->setCharacterSize(16);
    t->setColor(sf::Color::Black);
    t->setFont(font_);
    t->move(
      (game_->size().w * cell_width_ - 130)/2.,
      (game_->size().h * cell_width_ - 40)/2.
    );
  }
  welcome_text_.move(0, -40);

  // loading unclicked, mine, etc
  sf::Sprite sp_unclicked   (sprite_sheet_, sf::IntRect(0,0,16,16));
  sf::Sprite sp_blank       (sprite_sheet_, sf::IntRect(17,0,16,16));
  sf::Sprite sp_flagged     (sprite_sheet_, sf::IntRect(34,0,16,16));
  sf::Sprite sp_mine        (sprite_sheet_, sf::IntRect(85,0,16,16));
  sf::Sprite sp_mine_clicked(sprite_sheet_, sf::IntRect(102,0,16,16));
  sf::Sprite sp_false_flagged(sprite_sheet_, sf::IntRect(119,0,16,16));
  
  sprites_.emplace("unclicked",     sp_unclicked);
  sprites_.emplace("blank",         sp_blank);
  sprites_.emplace("flagged",       sp_flagged);
  sprites_.emplace("bombed",        sp_mine);
  sprites_.emplace("bomb-clicked",  sp_mine_clicked);
  sprites_.emplace("false-flagged", sp_false_flagged);
}

Minesweeper::Minesweeper(Game& game) : game_(&game)
{
  load_resources();
}

Minesweeper::~Minesweeper()
{
}
