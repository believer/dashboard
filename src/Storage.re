open Dom.Storage;

let hasConfig = item =>
  switch (localStorage |> getItem(item)) {
  | Some(_) => true
  | None => false
  };

let getConfig = item =>
  switch (localStorage |> getItem(item)) {
  | Some(value) => value
  | None => ""
  };

module GitHub = {
  type config = {interval: int};

  let config = {
    interval:
      (
        switch (localStorage |> getItem("github_interval")) {
        | Some("") => 60
        | Some(value) => int_of_string(value)
        | None => 60
        }
      )
      * 1000,
  };
};

module Trello = {
  type config = {interval: int};

  let config = {
    interval:
      (
        switch (localStorage |> getItem("trello_interval")) {
        | Some("") => 60
        | Some(value) => int_of_string(value)
        | None => 60
        }
      )
      * 1000,
  };
};
