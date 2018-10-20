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
