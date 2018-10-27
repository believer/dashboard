let component = ReasonReact.statelessComponent("SuccessMessage");

let make = children => {
  ...component,
  render: _self =>
    <div className="bg-light-green ba br2 bw1 b--green mb2 w-50 pa2 dark-gray">
      ...children
    </div>,
};
