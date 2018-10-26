let component = ReasonReact.statelessComponent("Header");

let make =
    (
      ~title,
      ~color="b--hot-pink",
      ~hasItems=false,
      ~markAllAsRead=?,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="flex justify-between items-start">
      <h2 className="f3 mt0 dark-gray">
        {title |> Utils.str}
        <div className={Cn.make(["w3 bt bw2 mt3", color])} />
      </h2>
      {
        switch (hasItems, markAllAsRead) {
        | (true, Some(onClick)) =>
          <SecondaryButton onClick>
            "Mark all as read"->Utils.str
          </SecondaryButton>
        | (true, None)
        | (false, Some(_))
        | (false, None) => ReasonReact.null
        }
      }
    </div>,
};
