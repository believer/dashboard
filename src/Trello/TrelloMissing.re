let component = ReasonReact.statelessComponent("TrelloMissing");

let make = _children => {
  ...component,
  render: _self =>
    <div className="lh-copy">
      <div className="f5 b mb2">
        {"Trello is missing configuration" |> Utils.str}
      </div>
      <div>
        {"Add the following values to your local storage" |> Utils.str}
      </div>
      <ul className="list pl0 mb0">
        <li className="mb2">
          <div className="bg-light-gray dib ph2 pv1 br2">
            {"trello_username" |> Utils.str}
          </div>
          {" - Your Trello username" |> Utils.str}
        </li>
        <li className="mb2">
          <div className="bg-light-gray dib ph2 pv1 br2">
            {"trello_key" |> Utils.str}
          </div>
          {" - find a key " |> Utils.str}
          <a
            className="link blue hover-hot-pink"
            href="https://trello.com/app-key">
            {"here" |> Utils.str}
          </a>
        </li>
        <li>
          <div className="bg-light-gray dib ph2 pv1 br2">
            {"trello_token" |> Utils.str}
          </div>
          {" - generate a token " |> Utils.str}
          <a
            className="link blue hover-hot-pink"
            href="https://trello.com/app-key">
            {"here" |> Utils.str}
          </a>
        </li>
      </ul>
    </div>,
};
