let component = ReasonReact.statelessComponent("Navigation");

let make = _children => {
  ...component,
  render: _self =>
    <div className="fixed left-0 top-0 bottom-0">
      <div className="bg-blue white pa4 flex justify-center">
        <div className="bt bw1 b--dashed w2" />
      </div>
      <div
        className="flex flex-column items-center bg-alabaster br h-100 b--black-10 pa4">
        <IconHome
          className="mb4"
          onClick={_ => ReasonReact.Router.push("/")}
        />
        <IconSettings onClick={_ => ReasonReact.Router.push("/#/settings")} />
      </div>
    </div>,
};
