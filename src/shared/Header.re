let component = ReasonReact.statelessComponent("Header");

let make = (~title, ~color="b--hot-pink", _children) => {
  ...component,
  render: _self =>
    <h2 className="f3 mt0 dark-gray">
      {title |> Utils.str}
      <div className={Cn.make(["w3 bt bw2 mt3", color])} />
    </h2>,
};
