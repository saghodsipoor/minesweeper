#include <algorithm>
#include <chrono>
#include <list>
#include <random>
#include <vector>

//test
#include <iostream>
//////
#include "Game.hpp"

void Game::reset()
{
  board_.clear();
  board_.resize(size_.w * size_.h);
  plant_bombs();
  set_cell_values();
  state_ = On;
}

inline bool Game::index_is_valid(const Index& index)
{
  return index.i >= 0 && index.i < size_.w && index.j >= 0 && index.j < size_.h;
}

const std::string Game::cell_state(const Index& index) const
{
  const auto& cell = board_[index.i * size_.w + index.j];
  
  if (cell.flagged)
    return "flagged";

  if (!cell.visitted)
    return "unclicked";

  // if visitted
  if (cell.bombed)
    return "bomb-clicked";

  if (cell.neighbor_bombs == 0)
    return "blank";

  return std::to_string(cell.neighbor_bombs);
}

void Game::toggle_flag(const Index& index)
{
  auto& cell = this->cell(index);
  if (!cell.visitted)
    cell.flagged = !cell.flagged;
  //dev
  set_game_state();
  /////
}

void Game::visit(const Index& index)
{
  auto first = index;
  if (cell(first).flagged)
    return;
  
  // if cell isn't flagged
  std::list<Index> to_visit {first};
  while(!to_visit.empty())
  {
    auto curr_index = to_visit.front();
    auto& curr_cell = cell(curr_index);
    curr_cell.visitted = true;

    if (curr_cell.bombed)
    {
      // state_ = Lost;
      break;
    }
    if (curr_cell.neighbor_bombs != 0)
    {
      to_visit.pop_front();
      continue;
    }
    
    // checking neighbors' condition
    for (const auto& dir : dirs_)
    {
      auto neighbor_index = curr_index + dir;
      if (!index_is_valid(neighbor_index))
        continue;
      
      if (cell(neighbor_index).flagged)
        continue;
      // blank cell
      if (!cell(neighbor_index).visitted)
        to_visit.push_back(neighbor_index);
    }
    to_visit.pop_front();
  }
  //dev
  set_game_state();
  /////
}

void Game::print()
{
  for (int i = 0; i < size_.w; ++i)
  {
    for (int j = 0; j < size_.h; ++j)
    {
      const auto& cell = this->cell({i,j});
      if (cell.flagged)
        std::cout << "\e[38;5;46m" << " f " << "\e[0m";
      else
      if (cell.bombed)
        std::cout << "\e[38;5;196m" << " b " << "\e[0m";
      else
      if (cell.visitted)
        std::cout << "\e[38;5;26m " << cell.neighbor_bombs << " \e[0m";
      else
        std::cout << " " << cell.neighbor_bombs << " ";
    }
    std::cout << std::endl;
  }
}

Game::Size Game::size() const 
{
  return size_;
}

void Game::set_cell_values()
{
  for_each_index([this](int i, int j){
    unsigned num = 0;
    for (const auto& dir : dirs_)
    {
      auto index = Index(i,j) + dir;
      if (!index_is_valid(index))
        { continue; }
      else
      if(cell(index).bombed)
        ++num;
    }
    cell({i,j}).neighbor_bombs = num;
  });
}

void Game::plant_bombs()
{
  bombs_num_ = size_.h * size_.w / 10;
  
  // make list of indices
  std::vector<Index> indices;

  for_each_index([&indices](int i, int j){
      indices.push_back({i,j});
  });  

  // shuffle indices
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(indices.begin(), indices.end(), std::default_random_engine(seed));

  // plant bombs for list of indices
  for (auto i = 0; i < bombs_num_; ++i)
    cell(indices[i]).bombed = true;
}

void Game::set_game_state()
{
  auto bombs_flagged = 0, non_bombs_visited = 0;
  auto fake_flagged = false;
  for (int i = 0; i < size_.w; ++i)
  {
    if (fake_flagged)
      break;
    for (int j = 0; j < size_.h; ++j)
    {
      auto& cell = this->cell({i,j});
      
      if (cell.visitted && cell.bombed)
      {
        state_ = Lost;
        return;
      }

      if (cell.visitted && !cell.bombed)
        ++non_bombs_visited;
      else
      if (cell.flagged && cell.bombed)
        ++bombs_flagged;
      else
      if (cell.flagged && !cell.bombed)
      {
        fake_flagged = true; 
        break;
      }
    }
  }
  if ((bombs_flagged == bombs_num_ && !fake_flagged) ||
    non_bombs_visited == size_.w * size_.h - bombs_num_)
    state_ = Won;
  //test
  std::cout << "flagged: " << bombs_flagged << '\n'; 
  std::cout << "visited: " << non_bombs_visited << '\n'; 
  std::cout << "faked:   " << std::string { !fake_flagged? "false" : "true"} << '\n'; 
  //////
}

Game::Game(Size size) : size_(size), board_(size.w * size.h)
{
  plant_bombs();
  set_cell_values();
}

Game::~Game()
{
}
